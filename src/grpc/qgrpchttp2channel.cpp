// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>, Viktor Kopp <vifactor@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:critical reason:network-protocol

#include <QtGrpc/private/qgrpcchanneloptions_p.h>
#include <QtGrpc/private/qtgrpclogging_p.h>
#include <QtGrpc/qgrpccalloptions.h>
#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpchttp2channel.h>
#include <QtGrpc/qgrpcinterceptorchain.h>
#include <QtGrpc/qgrpcoperationcontext.h>
#include <QtGrpc/qgrpcserializationformat.h>
#include <QtGrpc/qgrpcstatus.h>

#include <QtProtobuf/qprotobufjsonserializer.h>
#include <QtProtobuf/qprotobufserializer.h>

#include <QtNetwork/private/hpack_p.h>
#include <QtNetwork/private/http2protocol_p.h>
#include <QtNetwork/private/qdecompresshelper_p.h>
#include <QtNetwork/private/qhttp2connection_p.h>
#include <QtNetwork/qhttp2configuration.h>
#if QT_CONFIG(localserver)
#  include <QtNetwork/qlocalsocket.h>
#endif
#include <QtNetwork/qtcpsocket.h>
#if QT_CONFIG(ssl)
#  include <QtNetwork/qsslsocket.h>
#endif

#include <QtCore/private/qdebug_p.h>
#include <QtCore/private/qexpected_p.h>
#include <QtCore/private/qnoncontiguousbytedevice_p.h>
#include <QtCore/qalgorithms.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qendian.h>
#include <QtCore/qiodevice.h>
#include <QtCore/qlist.h>
#include <QtCore/qmetaobject.h>
#include <QtCore/qpointer.h>
#include <QtCore/qqueue.h>
#include <QtCore/qrandom.h>
#include <QtCore/qscopedvaluerollback.h>
#include <QtCore/qtenvironmentvariables.h>
#include <QtCore/qtimer.h>
#include <QtCore/qvarlengtharray.h>

#include <QtCore/q20algorithm.h>
#include <QtCore/q20utility.h>

#include <cmath>
#include <functional>
#include <optional>
#include <utility>

#include <zlib.h>

QT_BEGIN_NAMESPACE

using namespace Qt::Literals::StringLiterals;
using namespace QtGrpc;
using namespace std::chrono_literals;

/*!
    \class QGrpcHttp2Channel
    \inmodule QtGrpc
    \brief The QGrpcHttp2Channel class provides a HTTP/2 transport layer
    for \gRPC communication.

    The QGrpcHttp2Channel class implements QAbstractGrpcChannel, enabling \gRPC
    communication carried over \l{https://datatracker.ietf.org/doc/html/rfc7540}
    {HTTP/2 framing}.

    HTTP/2 introduces several advantages over its predecessor, HTTP/1.1, making
    QGrpcHttp2Channel well-suited for high-performance, real-time applications
    that require efficient communication, without sacrificing security or
    reliability, by using multiplexed TCP connections.

    The channel can be customized with \l{Secure Sockets Layer (SSL)
    Classes}{SSL} support, a custom \l{QGrpcChannelOptions::}
    {serializationFormat}, or other options by constructing it with a
    QGrpcChannelOptions containing the required customizations.

    \note \l{QGrpcChannelOptions::filterServerMetadata} is enabled by default.

    \section2 Transportation scheme

    The QGrpcHttp2Channel implementation prefers different transportation
    methods based on the provided \c{hostUri}, \l{QUrl::}{scheme} and options.
    The following criteria applies:

    \table
    \header
        \li Scheme
        \li Description
        \li Default Port
        \li Requirements
        \li Example
    \row
        \li \c{http}
        \li Unencrypted HTTP/2 over TCP
        \li 80
        \li None
        \li \c{http://localhost}
    \row
        \li \c{https}
        \li TLS-encrypted HTTP/2 over TCP
        \li 443
        \li QSslSocket support \b{AND} (scheme \b{OR} \l{QGrpcChannelOptions::}{sslConfiguration})
        \li \c{https://localhost}
    \row
        \li \c{unix}
        \li Unix domain socket in filesystem path
        \li ✗
        \li QLocalSocket support \b{AND} scheme
        \li \c{unix:///tmp/grpc.socket}
    \row
        \li \c{unix-abstract}
        \li Unix domain socket in abstract namespace
        \li ✗
        \li QLocalSocket support \b{AND}
            \l{QLocalSocket::AbstractNamespaceOption}{AbstractNamespace}
            support \b{AND} scheme
        \li \c{unix-abstract:app_grpc_channel}
    \endtable

    \section2 Content-Type

    The \e{content-type} in \gRPC over HTTP/2 determines the message
    serialization format. It must start with \c{application/grpc} and can
    include a suffix. The format follows this scheme:

    \code
        "content-type": "application/grpc" [("+proto" / "+json" / {custom})]
    \endcode

    For example:
    \list
        \li \c{application/grpc+proto} specifies Protobuf encoding.
        \li \c{application/grpc+json} specifies JSON encoding.
    \endlist

    The serialization format can be configured either by specifying the \c
    {content-type} inside the metadata or by setting the \l{QGrpcChannelOptions::}
    {serializationFormat} directly. By default, the \c {application/grpc}
    content-type is used.

    To configure QGrpcHttp2Channel with the JSON serialization format using
    \c {content-type} metadata:

    \code
        auto jsonChannel = std::make_shared<QGrpcHttp2Channel>(
            QUrl("http://localhost:50051"_L1),
            QGrpcChannelOptions().setMetadata({
                { "content-type"_ba, "application/grpc+json"_ba },
            })
        );
    \endcode

    For a custom serializer and \c {content-type}, you can directly set the
    serialization format:

    \include qgrpcserializationformat.cpp custom-serializer-code

    \code
        auto dummyChannel = std::make_shared<QGrpcHttp2Channel>(
            QUrl("http://localhost:50051"_L1),
            QGrpcChannelOptions().setSerializationFormat(dummyFormat)
        );
    \endcode

    \include qgrpcserializationformat.cpp custom-serializer-desc

    \section2 Reserved metadata keys

    Metadata is transmitted as HTTP/2 headers: \b{keys} are case-insensitive
    ASCII strings, \b{values} may be ASCII strings or binary data. The
    following keys are reserved by HTTP/2 or the \gRPC protocol and are
    dropped from user metadata when the request is built:

    \list
        \li HTTP/2 pseudo-headers (any key starting with \c{:}).
        \li Any key with the \c{grpc-} or \c{qtgrpc-} prefix.
        \li \c{te}, \c{content-type}, \c{user-agent}.
    \endlist

    A user-provided \c{content-type} is still consulted at channel construction
    for serializer auto-detection (see \l{Content-Type}); it is not, however,
    forwarded as a Custom-Metadata entry.

    For more information on HTTP/2 headers, see
    \l{https://www.rfc-editor.org/rfc/rfc7540.html#section-8.1.2}{RFC 7540,
    Section 8.1.2}.

    \section2 Receive windows

    HTTP/2 flow control limits how much data a sender may transmit before the
    receiver acknowledges it. The channel advertises two such limits to the
    server, the \e{receive windows}:

    \list
        \li The \e{stream window} caps the unacknowledged data of a single
            RPC.
        \li The \e{connection window} caps the combined unacknowledged data
            of all RPCs, which share one connection.
    \endlist

    A window limits data in flight, not the size of a transfer. Because
    acknowledgements take one network round trip, it also caps throughput:

    \badcode
    maximum throughput = window / round-trip time
    \endcode

    Both windows are configured in bytes through environment variables:

    \list
        \li \c QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE for the stream windows;
            (defaults to \c{4 MiB})
        \li \c QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE for the connection
            window; (defaults to four times the stream window, \c{16 MiB})
    \endlist

    Setting only \c QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE automatically
    scales the connection window to four times the stream window. Set \c
    QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE explicitly to use a
    different ratio.

    The defaults saturate local networks and most internet paths. They fall
    short on fast links with long round-trip times. For example, an
    application receives large messages from a distant server:

    \badcode
    link: 1 Gbit/s (125 MB/s), 100 ms round-trip time
    \endcode

    The server sends one stream window of data, then waits one round trip
    for the acknowledgement. With the default window it delivers at most
    4 MiB every 100 ms:

    \badcode
    4 MiB / 0.1 s = 42 MB/s, 34% of the 125 MB/s the link carries
    \endcode

    To saturate the link, the window must hold everything the link delivers
    during one round trip, the link's \e{bandwidth-delay product} (BDP):

    \badcode
    BDP = 125 MB/s x 0.1 s = 12.5 MB

    QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE=12500000
    12.5 MB / 0.1 s = 125 MB/s, the link is saturated
    \endcode

    Concurrent RPCs share a single connection receive window. As long as every
    stream is continuously read, the total in-flight data remains bounded by
    the BDP. If a stream is not read, however, its buffered data consumes part
    of the shared connection window and may block other streams. A good rule of
    thumb is to size the connection window for the sum of all concurrently
    active stream windows. For eight parallel transfers on the link above:

    \badcode
    QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE=12500000       (12.5 MB = BDP)
    QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE=100000000  (8 x 12.5 MB)
    \endcode

    Each RPC can use the full 125 MB/s when it is the only active transfer.
    With eight concurrent RPCs, they share the link at roughly 15 MB/s each. In
    the worst case, if the application stops reading from every stream, the
    channel may buffer up to 100 MB of data.

    Alternatively, size the stream windows so they collectively fit within a
    connection window equal to the BDP. This bounds the maximum buffered data,
    but limits every RPC to its allocated share, even when it transfers alone:

    \badcode
    QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE=1562500        (12.5 MB / 8)
    QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE=12500000   (= BDP)
    \endcode

    Each RPC is limited to roughly 15 MB/s, whether it transfers alone or
    alongside seven others. The benefit is that the channel never buffers more
    than about 12.5 MB of unread data.

    Use full-sized stream windows when minimizing transfer latency is more
    important than memory usage. Use smaller stream windows when bounding
    memory consumption is the priority. In either case, keep the connection
    window large enough to hold several stream windows; otherwise a single
    unread stream can exhaust the shared connection window and block all other
    RPCs.

    Both windows accept values up to \c{2147483647} bytes. The stream window
    has a floor of \c{1024} bytes, which lets memory-constrained receivers
    bound per-stream buffering; the connection window has a floor of
    \c{65535} bytes, the protocol's initial window.

    \section2 Environment variable fallbacks

    Some channel options can be configured through environment variables.
    Environment variables are only consulted if the corresponding option is not
    set explicitly. They are evaluated when each channel is constructed. If
    neither an explicit option nor an environment variable is provided, the
    built-in default is used.

    \table
        \header
            \li Environment Variable
            \li Option
            \li Default fallback
        \row
            \li \c QT_GRPC_MAXIMUM_RECEIVE_MESSAGE_SIZE
            \li \l{QGrpcChannelOptions::}{maximumReceiveMessageSize}
            \li 4 MiB (4'194'304 Bytes)
        \row
            \li \c QT_GRPC_MAXIMUM_METADATA_SIZE
            \li \l{QGrpcChannelOptions::maximumMetadataSize}
            \li 16 KiB (16'384 Bytes)
        \row
            \li \c QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS
            \li \l{QGrpcChannelOptions::}{initialReconnectBackoff}
            \li 1s (1'000ms)
        \row
            \li \c QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS
            \li \l{QGrpcChannelOptions::}{maximumReconnectBackoff}
            \li 120s (120'000ms)
        \row
            \li \c QT_GRPC_CONNECT_TIMEOUT_MS
            \li \l{QGrpcChannelOptions::}{connectTimeout}
            \li 20s (20'000ms)
        \row
            \li \c QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE
            \li \e{None}
            \li 4 MiB (4'194'304 Bytes)
        \row
            \li \c QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE
            \li \e{None}
            \li 16 MiB (16'777'216 Bytes)
    \endtable

    \sa QAbstractGrpcChannel, QGrpcChannelOptions, QGrpcSerializationFormat
*/

namespace {

Q_STATIC_LOGGING_CATEGORY(lcChannel, "qt.grpc.channel.http2")
Q_STATIC_LOGGING_CATEGORY(lcStream, "qt.grpc.channel.http2.stream")

constexpr QLatin1String UnixScheme("unix");
constexpr QLatin1String UnixAbstractScheme("unix-abstract");
constexpr QLatin1String HttpScheme("http");
constexpr QLatin1String HttpsScheme("https");

const QByteArray HttpStatusHeader(":status");
const QByteArray ContentTypeHeader("content-type");
const QByteArray GrpcStatusHeader("grpc-status");
const QByteArray GrpcStatusMessageHeader("grpc-message");
const QByteArray DefaultContentType("application/grpc");
const QByteArray GrpcStatusDetailsHeader("grpc-status-details-bin");
const QByteArray GrpcAcceptEncodingHeader("grpc-accept-encoding");
const QByteArray GrpcEncodingHeader("grpc-encoding");
constexpr qsizetype GrpcMessageSizeHeaderSize = 5;
constexpr quint8 GrpcCompressedFlag = 0x01;

// Maximum gRPC frame payload size for the HTTP/2 transport. Bounded by the
// wire-format u32 length and by qsizetype headroom for the 5-byte frame prefix
// on 32-bit platforms. Both bounds fit in quint32.
constexpr quint32
    GrpcMaxPayloadSize = quint32((std::min)(quint64{ (std::numeric_limits<quint32>::max)() },
                                            quint64{ (std::numeric_limits<qsizetype>::max)() }
                                                - GrpcMessageSizeHeaderSize));

constexpr const char EnvMaximumReceiveMessageSize[] = "QT_GRPC_MAXIMUM_RECEIVE_MESSAGE_SIZE";
constexpr quint32 DefaultMaximumReceiveMessageSize = 4 * 1024 * 1024; // 4 MiB

constexpr const char EnvHttp2StreamReceiveWindowSize[] = "QT_GRPC_HTTP2_STREAM_RECEIVE_WINDOW_SIZE";
constexpr const char
    EnvHttp2ConnReceiveWindowSize[] = "QT_GRPC_HTTP2_CONNECTION_RECEIVE_WINDOW_SIZE";
constexpr quint32 DefaultHttp2StreamReceiveWindowSize = 4 * 1024 * 1024; // 4 MiB
// RFC 6.9.3 permits arbitrarily small per-stream windows
constexpr quint32 MinimumHttp2StreamReceiveWindowSize = 1024; // 1 KiB

constexpr const char EnvInitialReconnectBackoff[] = "QT_GRPC_INITIAL_RECONNECT_BACKOFF_MS";
constexpr const char EnvMaximumReconnectBackoff[] = "QT_GRPC_MAXIMUM_RECONNECT_BACKOFF_MS";
constexpr const char EnvConnectTimeout[] = "QT_GRPC_CONNECT_TIMEOUT_MS";

constexpr const char EnvGrpcMaximumMetadataSize[] = "QT_GRPC_MAXIMUM_METADATA_SIZE";

// Soft diagnostic threshold only; the queue stays unbounded.
constexpr quint32 QueuedBytesWarningThreshold = 16 * 1024 * 1024; // 16 MiB

std::optional<quint64> readEnvUnsignedInt(const char *name)
{
    const auto v = qEnvironmentVariableIntegerValue(name);
    if (!v)
        return std::nullopt;

    if (*v < 0) {
        qCWarning(lcChannel, "%s has an invalid value (%lld); ignoring.", name,
                  static_cast<long long>(*v));
        return std::nullopt;
    }

    return static_cast<quint64>(*v);
}

// Clamps a quint64 byte limit into the 32-bit HTTP/2 transport cap [floor, cap]
quint32 clampToRange(quint64 configured, quint32 floor, quint32 cap, const char *source)
{
    Q_PRE(floor <= cap);
    if (configured > cap) {
        qCWarning(lcChannel,
                  "Configured size %llu (%s) exceeds the HTTP/2 transport cap of "
                  "%u bytes; clamping to the cap.",
                  configured, source, cap);
        return cap;
    }
    if (configured < floor) {
        qCWarning(lcChannel,
                  "Configured size %llu (%s) is below the minimum of %u bytes; "
                  "raising to the minimum.",
                  configured, source, floor);
        return floor;
    }
    return quint32(configured);
}

// This HTTP/2 Error Codes to QGrpcStatus::StatusCode mapping should be kept in sync
// with the following docs:
//     https://www.rfc-editor.org/rfc/rfc7540#section-7
//     https://github.com/grpc/grpc/blob/master/doc/statuscodes.md
//     https://github.com/grpc/grpc/blob/master/doc/PROTOCOL-HTTP2.md
constexpr StatusCode http2ErrorToStatusCode(const quint32 http2Error)
{
    using Http2Error = Http2::Http2Error;

    switch (http2Error) {
    case Http2Error::HTTP2_NO_ERROR:
    case Http2Error::PROTOCOL_ERROR:
    case Http2Error::INTERNAL_ERROR:
    case Http2Error::FLOW_CONTROL_ERROR:
    case Http2Error::SETTINGS_TIMEOUT:
    case Http2Error::STREAM_CLOSED:
    case Http2Error::FRAME_SIZE_ERROR:
        return StatusCode::Internal;
    case Http2Error::REFUSE_STREAM:
        return StatusCode::Unavailable;
    case Http2Error::CANCEL:
        return StatusCode::Cancelled;
    case Http2Error::COMPRESSION_ERROR:
    case Http2Error::CONNECT_ERROR:
        return StatusCode::Internal;
    case Http2Error::ENHANCE_YOUR_CALM:
        return StatusCode::ResourceExhausted;
    case Http2Error::INADEQUATE_SECURITY:
        return StatusCode::PermissionDenied;
    case Http2Error::HTTP_1_1_REQUIRED:
        return StatusCode::Unknown;
    }
    return StatusCode::Internal;
}

// Ref: https://github.com/grpc/grpc/blob/master/doc/http-grpc-status-mapping.md
constexpr StatusCode http2StatusToStatusCode(const int status)
{
    switch (status) {
    case 200:
        return StatusCode::Ok;
    case 400:
        return StatusCode::Internal;
    case 401:
        return StatusCode::Unauthenticated;
    case 403:
        return StatusCode::PermissionDenied;
    case 404:
        return StatusCode::Unimplemented;
    case 429:
    case 502:
    case 503:
    case 504:
        return StatusCode::Unavailable;
    default:
        return StatusCode::Unknown;
    }
}

QByteArray requestEncodingName(CompressionAlgorithm algo)
{
    Q_ASSERT(algo != CompressionAlgorithm::Identity);
    return algo == CompressionAlgorithm::Gzip ? "gzip"_ba : "deflate"_ba;
}

bool isAcceptedEncoding(QByteArrayView v, QtGrpc::CompressionAlgorithms accepted)
{
    if (v == "identity"_ba)
        return true;
    if (v == "gzip"_ba)
        return accepted & CompressionAlgorithm::Gzip;
    if (v == "deflate"_ba)
        return accepted & CompressionAlgorithm::Deflate;
    return false;
}

bool hasSslConfiguration(const QGrpcChannelOptions &opts)
{
#if QT_CONFIG(ssl)
    return opts.sslConfiguration().has_value();
#else
    Q_UNUSED(opts)
    return false;
#endif
}

QByteArray sanitizedForLog(QByteArrayView v)
{
    // Untrusted strings should be bounded and escaped before
    // logging to prevent log injection (CWE-117).
    constexpr qsizetype MaxLoggedHeaderBytes = 256;
    return QtDebugUtils::toPrintable(v.data(), v.size(), MaxLoggedHeaderBytes);
}

std::chrono::milliseconds resolveMs(std::optional<std::chrono::milliseconds> val,
                                    std::optional<quint64> envVal, std::chrono::milliseconds dflt)
{
    if (val)
        return *val;
    if (envVal)
        return std::chrono::milliseconds(*envVal);
    return dflt;
}

} // namespace

class GrpcDataParser
{
public:
    struct Frame
    {
        Frame(QByteArray &&payload, bool isCompressed) noexcept
            : payload(std::move(payload)), isCompressed(isCompressed)
        {
        }
        QByteArray payload;
        bool isCompressed = false;
    };

    struct ParseError
    {
        enum Kind {
            NeedMoreData, // Buffer does not yet contain a full frame.
            Oversize, // Declared length exceeds maxReceiveSize.
        };
        Kind kind;
        quint32 declaredSize; // Valid for Oversize.
    };

    explicit GrpcDataParser(quint32 maxReceiveSize) : maxReceiveSize(maxReceiveSize) {}

    const quint32 maxReceiveSize;

    [[nodiscard]] q23::expected<Frame, ParseError> parseNextFrame()
    {
        static constexpr qsizetype FlagOffset = 0;
        static constexpr qsizetype LengthOffset = 1;

        auto out = q23::expected<Frame, ParseError>{ q23::unexpected(ParseError{
            ParseError::NeedMoreData, 0 }) };

        if (container.size() < GrpcMessageSizeHeaderSize)
            return out;

        // Parse length (big endian, 4 bytes after flag)
        const auto messageLength = qFromBigEndian<
            quint32>(reinterpret_cast<const uchar *>(container.constData() + LengthOffset));

        // Reject frames that exceed the configured receive size limit before
        // allocating any memory for the payload.
        if (messageLength > maxReceiveSize) {
            // The stream will be aborted and cleaned up asynchronously after
            // this error; discard the accumulated buffer now as no further
            // frames will be processed.
            container.clear();
            out = q23::unexpected(ParseError{ ParseError::Oversize, messageLength });
            return out;
        }

        // Safe on 32-bit platforms: maxReceiveSize is clamped to GrpcMaxPayloadSize
        const qsizetype frameSize = GrpcMessageSizeHeaderSize + messageLength;

        if (container.size() < frameSize)
            return out;

        out.emplace(container.mid(GrpcMessageSizeHeaderSize, messageLength),
                    container.at(FlagOffset) != 0);
        container.remove(0, frameSize);
        return out;
    }

    void feed(const QByteArray &data) { container.append(data); }
    qsizetype bytesAvailable() const { return container.size(); }

private:
    QByteArray container;
};

// Encapsulates a zlib deflate stream used to compress outbound gRPC messages.
class GrpcCompressionHelper
{
    QT_DEFINE_TAG_STRUCT(PrivateConstructor);

public:
    enum class Error {
        // Compression was not applied: the payload is uncompressible or the
        // worst-case bound exceeds the frame cap. The helper is still usable;
        // the caller should fall back to sending the payload uncompressed.
        Skipped,
        // Hard zlib failure: the zstream is in an undefined state and the
        // helper must be discarded so a fresh one is created on retry.
        Broken,
    };

    explicit GrpcCompressionHelper(PrivateConstructor) {}
    ~GrpcCompressionHelper() { deflateEnd(&m_stream); }

    // Lazily initializes the zstream; returns nullptr on initialization failure.
    static std::unique_ptr<GrpcCompressionHelper> create(CompressionAlgorithm algo)
    {
        auto helper = std::make_unique<GrpcCompressionHelper>(PrivateConstructor{});
        const auto res = deflateInit2(&helper->m_stream, Z_BEST_SPEED, Z_DEFLATED,
                                      windowBitsFor(algo), 8, Z_DEFAULT_STRATEGY);
        if (res != Z_OK) {
            qCWarning(lcStream,
                      "gRPC request compression: deflateInit2 failed (ret=%d, %s, msg=%s)", res,
                      zError(res), helper->m_stream.msg ? helper->m_stream.msg : "n/a");
            return nullptr;
        }
        return helper;
    }

    // Compresses \a in directly into \a out, reserving space for
    // GrpcMessageSizeHeaderSize. On success returns the compressed payload
    // size.
    [[nodiscard]] q23::expected<qsizetype, Error> tryCompressMessage(QByteArrayView in,
                                                                     QByteArray &out)
    {
        Q_ASSERT(!in.isEmpty());
        Q_ASSERT(q20::cmp_less_equal(in.size(), GrpcMaxPayloadSize));

        // Qt requires 32-bit int globally. Restate the assumption for zlib's uInt.
        static_assert(std::numeric_limits<uInt>::max() >= std::numeric_limits<quint32>::max(),
                      "zlib uInt must be at least 32 bits");

        if (m_needsReset) { // first call skips deflateReset
            if (const int res = deflateReset(&m_stream); res != Z_OK) {
                qCWarning(lcStream,
                          "[%p] gRPC request compression: deflateReset failed (ret=%d, %s, msg=%s)",
                          static_cast<const void *>(this), res, zError(res),
                          m_stream.msg ? m_stream.msg : "n/a");
                return q23::unexpected(Error::Broken);
            }
            m_needsReset = false;
        }

        // Size the output to the input length. deflate(Z_FINISH) then returns
        // Z_STREAM_END if compression shrunk the payload; when it doesn't fit,
        // zlib documents Z_OK or Z_BUF_ERROR as the non-fatal "needs more
        // output space" signals that we use as our skip trigger.
        out = QByteArray(GrpcMessageSizeHeaderSize + in.size(), Qt::Uninitialized);

        // zlib supports const next_in via z_const, but it's disabled by default.
        m_stream.next_in = reinterpret_cast<Bytef *>(const_cast<char *>(in.data()));
        m_stream.avail_in = static_cast<uInt>(in.size()); // fits into uInt
        m_stream.next_out = reinterpret_cast<Bytef *>(out.data() + GrpcMessageSizeHeaderSize);
        m_stream.avail_out = static_cast<uInt>(in.size()); // fits into uInt

        const int ret = deflate(&m_stream, Z_FINISH);
        // Either outcome leaves the stream non-reusable; the next call must reset.
        m_needsReset = true;
        if (ret == Z_OK || ret == Z_BUF_ERROR)
            return q23::unexpected(Error::Skipped);
        if (ret != Z_STREAM_END) {
            qCWarning(lcStream,
                      "[%p] gRPC request compression: deflate failed (ret=%d, %s, msg=%s)",
                      static_cast<const void *>(this), ret, zError(ret),
                      m_stream.msg ? m_stream.msg : "n/a");
            return q23::unexpected(Error::Broken);
        }

        const auto compressedSize = static_cast<qsizetype>(m_stream.total_out);
        // Adjust the logical size to the actual compressed payload.
        out.resize(GrpcMessageSizeHeaderSize + compressedSize);
        return compressedSize;
    }

private:
    static constexpr int windowBitsFor(CompressionAlgorithm algo)
    {
        Q_ASSERT(algo != CompressionAlgorithm::Identity);
        return algo == CompressionAlgorithm::Gzip ? (15 | 16) : 15;
    }

    z_stream m_stream{};
    bool m_needsReset = false;

    Q_DISABLE_COPY_MOVE(GrpcCompressionHelper)
};

struct ReconnectBackoff
{
    ReconnectBackoff(std::chrono::milliseconds init, std::chrono::milliseconds max)
        : m_current(init), m_initial(init), m_maximum(max)
    {
    }

    void reset() { m_current = m_initial; }

    [[nodiscard]] std::chrono::milliseconds nextDelay()
    {
        constexpr double BackoffMultiplier = 1.6;
        constexpr double BackoffJitter = 0.2;

        const auto delay = m_current;
        // Grow the underlying backoff value without jitter so retries don't
        // drift randomly over time. 0 stays 0 (backoff disabled) by design.
        // Clamp in double space: casting a value beyond INT64_MAX to qint64 is UB.
        const double multiplied = std::ceil(static_cast<double>(m_current.count())
                                            * BackoffMultiplier);
        m_current = multiplied >= static_cast<double>(m_maximum.count())
            ? m_maximum
            : std::chrono::milliseconds(static_cast<qint64>(multiplied));

        // Apply a random jitter in the range [0.8, 1.2] to avoid synchronized reconnects.
        const double jitterFactor = 1.0 - BackoffJitter
            + (2.0 * BackoffJitter * QRandomGenerator::global()->generateDouble());

        const double jittered = static_cast<double>(delay.count()) * jitterFactor;
        return jittered >= static_cast<double>(std::chrono::milliseconds::max().count())
            ? std::chrono::milliseconds::max()
            : std::chrono::milliseconds(static_cast<qint64>(jittered));
    }

private:
    std::chrono::milliseconds m_current;
    std::chrono::milliseconds m_initial;
    std::chrono::milliseconds m_maximum;
};

// The Http2Handler manages an individual RPC over the HTTP/2 channel.
// Each instance corresponds to an RPC initiated by the user.
class Http2Handler : public QObject
{
    Q_OBJECT

    enum class HeaderPhase { Invalid, Initial, Trailers, TrailersOnly };
    Q_ENUM(HeaderPhase);

public:
    enum class State : uint8_t {
        Idle,
        RequestHeadersSent,
        Active,
        // Endpoints
        Cancelled,
        Finished,
    };
    Q_ENUM(State);

    explicit Http2Handler(QGrpcHttp2ChannelPrivate *parent, QGrpcOperationContext *context,
                          QByteArray &&messageData, bool endStream);
    ~Http2Handler() override;

    void sendInitialRequest();
    void attachStream(QHttp2Stream *stream_);
    void processQueue();

    void finish(const QGrpcStatus &status);
    void asyncFinish(const QGrpcStatus &status);
    void cancelWithStatus(const QGrpcStatus &status);

    [[nodiscard]] bool expired() const { return !m_context; }

    [[nodiscard]] bool isStreamClosedForSending() const
    {
        // If stream pointer is nullptr this means we never opened it and should collect
        // the incoming messages in queue until the stream is opened or the error occurred.
        return m_stream != nullptr
            && (m_stream->state() == QHttp2Stream::State::HalfClosedLocal
                || m_stream->state() == QHttp2Stream::State::Closed);
    }

// context slot handlers:
    void cancel() { cancelWithStatus({ StatusCode::Cancelled, tr("Cancelled by client") }); }
    void writesDone();
    void writeMessage(QByteArrayView data);
    void deadlineTimeout()
    {
        cancelWithStatus({ StatusCode::DeadlineExceeded, tr("Deadline exceeded") });
    }

    void handleHeaders(const HPack::HttpHeader &headers, HeaderPhase phase);

private:
    [[nodiscard]] HPack::HttpHeader constructInitialHeaders() const;
    [[nodiscard]] bool constructFilterServerMetadata() const;
    [[nodiscard]] QGrpcHttp2ChannelPrivate *channelPriv() const;
    [[nodiscard]] QGrpcHttp2Channel *channel() const;
    [[nodiscard]] bool handleContextExpired();
    void scheduleMessageWritten();

    [[nodiscard]] static CompressionAlgorithm
    constructRequestEncoding(const QGrpcHttp2ChannelPrivate &channel,
                             const QGrpcOperationContext &context);
    [[nodiscard]] static quint32
    constructMaximumReceiveSize(const QGrpcHttp2ChannelPrivate &channel,
                                const QGrpcOperationContext &context);

    QPointer<QGrpcOperationContext> m_context;
    HPack::HttpHeader m_initialHeaders;
    QQueue<QByteArray> m_queue;
    QPointer<QHttp2Stream> m_stream;
    GrpcDataParser m_grpcDataParser;
    QByteArray m_negotiatedEncoding;
    std::unique_ptr<QDecompressHelper> m_decompressor;
    std::unique_ptr<GrpcCompressionHelper> m_compressor;
    State m_state = State::Idle;
    const bool m_endStreamAtFirstData;
    bool m_writesDoneSent = false;
    bool m_drainingQueue = false;
    bool m_inFlightUserWrite = false;
    quint64 m_queuedBytes = 0;
    quint64 m_nextQueueWarnBytes = QueuedBytesWarningThreshold;
    quint64 m_bytesToWrite = 0;
    bool m_filterServerMetadata;
    QTimer m_deadlineTimer;
    const CompressionAlgorithm m_requestEncoding;

    Q_DISABLE_COPY_MOVE(Http2Handler)
};

class QGrpcHttp2ChannelPrivate : public QObject
{
    Q_OBJECT
public:
    enum class SocketType : uint8_t { Tcp, Tls, Local, LocalAbstract };

    explicit QGrpcHttp2ChannelPrivate(const QUrl &uri, QGrpcHttp2Channel *q);
    ~QGrpcHttp2ChannelPrivate() override;

    void processOperation(QGrpcOperationContext *operationContext, QByteArray &&data,
                          bool endStream = false);

    QGrpcHttp2Channel *q_ptr = nullptr;
    const SocketType socketType;
    const QUrl hostUri;
    const QByteArray contentType;
    const QByteArray authorityHeader;
    const QByteArray schemeHeader;
    // Cached once at channel construction; the env var is process-global and
    // qEnvironmentVariable() takes a global lock per call.
    const std::optional<quint64>
        envMaximumReceiveSize = readEnvUnsignedInt(EnvMaximumReceiveMessageSize);
    const std::optional<quint64>
        envInitialReconnectBackoff = readEnvUnsignedInt(EnvInitialReconnectBackoff);
    const std::optional<quint64>
        envMaximumReconnectBackoff = readEnvUnsignedInt(EnvMaximumReconnectBackoff);
    const std::optional<quint64> envConnectTimeout = readEnvUnsignedInt(EnvConnectTimeout);
    const std::optional<quint64>
        envMaximumMetadataSize = readEnvUnsignedInt(EnvGrpcMaximumMetadataSize);

    [[nodiscard]] const QByteArray &acceptEncoding();

private:
    enum ConnectionState { Connecting = 0, Connected, SettingsReceived, Error };

    static SocketType constructSocketType(const QUrl &rawUri, const QGrpcChannelOptions &chOpts);
    QUrl sanitizeHostUri(const QUrl &rawUri, const QGrpcChannelOptions &chOpts) const;
    QByteArray setupContentTypeNegotiation(QGrpcHttp2Channel *qPtr) const;
    static QByteArray constructAuthorityHeader(const QUrl &hostUri, SocketType socketType);
    static QByteArray constructSchemeHeader(SocketType socketType);
    static ReconnectBackoff constructReconnectBackoff(const QGrpcChannelOptions &chOpts,
                                                      std::optional<quint64> envInitial,
                                                      std::optional<quint64> envMaximum);
    static QByteArray constructAcceptEncoding(QtGrpc::CompressionAlgorithms flags);

    struct ReceiveWindowSizes
    {
        quint32 stream;
        quint32 connection;
    };
    [[nodiscard]] static ReceiveWindowSizes constructReceiveWindowSizes();
    [[nodiscard]] static QHttp2Configuration
    createHttp2Configuration(const QGrpcHttp2ChannelPrivate &channel);

    bool createHttp2Stream(Http2Handler *handler);
    void createHttp2Connection();
    void attemptConnect();

#if QT_CONFIG(localserver)
    void handleLocalSocketError(QLocalSocket::LocalSocketError error)
    {
        handleSocketError(QDebug::toBytes(error));
    }
#endif
    void handleAbstractSocketError(QAbstractSocket::SocketError error)
    {
        handleSocketError(QDebug::toBytes(error));
    }
    void handleSocketError(const QByteArray &errorCode);

    // Invokes fn with m_socket downcast to its concrete socket type.
    template <typename Fn>
    auto visitSocket(Fn &&fn)
    {
#if QT_CONFIG(localserver)
        if (socketType == SocketType::Local || socketType == SocketType::LocalAbstract)
            return std::invoke(std::forward<Fn>(fn), static_cast<QLocalSocket *>(m_socket.get()));
#endif
        return std::invoke(std::forward<Fn>(fn), static_cast<QAbstractSocket *>(m_socket.get()));
    }

    template <typename Projection = q20::identity>
    void for_each_non_expired_handler(Projection proj)
    {
        QVarLengthArray<QObject *> expiredHandler;
        for (QObject *child : children()) {
            auto *handler = qobject_cast<Http2Handler *>(child);
            if (!handler)
                continue;
            if (handler->expired()) {
                expiredHandler.push_back(handler);
                continue;
            }
            std::invoke(std::forward<Projection>(proj), handler);
        }
        // Perform deletions after the loop to avoid modifying the children
        // list during iteration. Delete in reverse order to avoid
        // quadratic-time updates in QObject's children list.
        qDeleteAll(expiredHandler.crbegin(), expiredHandler.crend());
    }

private:
    std::unique_ptr<QIODevice> m_socket = nullptr;
    std::function<void()> m_reconnectFunction;

    bool m_isInsideSocketErrorOccurred = false;
    QHttp2Connection *m_connection = nullptr;
    ConnectionState m_state = Connecting;
    ReconnectBackoff m_reconnectBackoff;
    QTimer m_reconnectTimer;
    QTimer m_connectTimeoutTimer;

    // Lazily-built cache of the grpc-accept-encoding header value. Rebuilt
    // only when the configured algorithm flags change.
    QByteArray m_acceptEncoding;
    QtGrpc::CompressionAlgorithms m_acceptEncodingFlags;

    Q_DISABLE_COPY_MOVE(QGrpcHttp2ChannelPrivate)
};

///
/// ## Http2Handler Implementations
///

Http2Handler::Http2Handler(QGrpcHttp2ChannelPrivate *parent, QGrpcOperationContext *context,
                           QByteArray &&messageData, bool endStream)
    : QObject(parent), m_context(context),
      m_grpcDataParser(constructMaximumReceiveSize(*parent, *context)),
      m_endStreamAtFirstData(endStream), m_filterServerMetadata(constructFilterServerMetadata()),
      m_requestEncoding(constructRequestEncoding(*parent, *context))
{
    // constructInitialHeaders() depends on members initialized above; build it
    // here so it sees a fully constructed object.
    m_initialHeaders = constructInitialHeaders();

    // If the context (lifetime bound to the user) is destroyed, this handler
    // can no longer perform any meaningful work. We allow it to be deleted;
    // QHttp2Stream will handle any outstanding cancellations appropriately.
    connect(context, &QGrpcOperationContext::destroyed, this, &Http2Handler::deleteLater);
    connect(context, &QGrpcOperationContext::cancelRequested, this, &Http2Handler::cancel);
    connect(context, &QGrpcOperationContext::writesDoneRequested, this, &Http2Handler::writesDone);
    if (!m_endStreamAtFirstData) {
        connect(context, &QGrpcOperationContext::writeMessageRequested, this,
                &Http2Handler::writeMessage);
    }

    m_deadlineTimer.setSingleShot(true);

    writeMessage(std::move(messageData));
}

Http2Handler::~Http2Handler()
{
    qCDebug(lcStream, "[%p] Destroying Http2Handler (state=%s, stream=%p)", this,
            QDebug::toBytes(m_state).constData(), m_stream.get());
    if (m_stream) {
        QHttp2Stream *streamPtr = m_stream.get();
        m_stream.clear();
        delete streamPtr;
    }
}

// Attaches the HTTP/2 stream and sets up the necessary connections and
// preconditions. The HTTP/2 connection is established, and the transport
// is now ready for communication.
void Http2Handler::attachStream(QHttp2Stream *stream_)
{
    Q_ASSERT(m_stream == nullptr);
    Q_ASSERT(stream_ != nullptr);

    m_stream = stream_;

    connect(m_stream.get(), &QHttp2Stream::headersReceived, this,
            [this](const HPack::HttpHeader &headers, bool endStream) mutable {
                if (m_state >= State::Cancelled) {
                    // In case we are Cancelled or Finished, a
                    // finished has been emitted already and the
                    // Handler should get deleted here.
                    qCDebug(lcStream, "[%p] Ignoring headers - already closed (state=%s)", this,
                            QDebug::toBytes(m_state).constData());
                    deleteLater();
                    return;
                }

                HeaderPhase phase = HeaderPhase::Invalid;
                if (m_state == State::RequestHeadersSent && endStream)
                    phase = HeaderPhase::TrailersOnly;
                else if (m_state == State::RequestHeadersSent && !endStream)
                    phase = HeaderPhase::Initial;
                else if (m_state == State::Active && endStream) {
                    phase = HeaderPhase::Trailers;
                } else {
                    qCWarning(lcStream,
                              "[%p] Received unexpected %s HEADERS (state=%s, "
                              "endStream=%d)",
                              this, QDebug::toBytes(phase).constData(),
                              QDebug::toBytes(m_state).constData(), endStream);
                    return;
                }

                m_state = State::Active;
                handleHeaders(headers, phase);
            });

    connect(
        m_stream.get(), &QHttp2Stream::errorOccurred, this,
        [this](quint32 http2ErrorCode, const QString &errorString) {
            qCDebug(lcStream, "[%p] Stream errorOccurred (state=%s)", this,
                    QDebug::toBytes(m_state).constData());
            finish({ http2ErrorToStatusCode(http2ErrorCode), errorString });
        },
        Qt::SingleShotConnection);

    connect(m_stream.get(), &QHttp2Stream::dataReceived, m_context.get(),
            [this](const QByteArray &data, bool endStream) {
                if (m_state == State::Cancelled)
                    return;

                m_grpcDataParser.feed(data);
                while (true) {
                    auto frame = m_grpcDataParser.parseNextFrame();
                    if (!frame) {
                        switch (frame.error().kind) {
                        case GrpcDataParser::ParseError::NeedMoreData:
                            break;
                        case GrpcDataParser::ParseError::Oversize:
                            finish({ StatusCode::ResourceExhausted,
                                     QString::asprintf("Received message size (%u bytes) exceeds "
                                                       "configured limit (%u bytes)",
                                                       frame.error().declaredSize,
                                                       m_grpcDataParser.maxReceiveSize) });
                            return;
                        }
                        break;
                    }
                    QByteArray finalPayload;

                    if (frame->isCompressed) {
                        if (!m_decompressor || m_negotiatedEncoding.isEmpty()) {
                            finish({ QtGrpc::StatusCode::Internal,
                                     "Protocol error: received compressed message "
                                     "but no encoding was negotiated." });
                            return;
                        }
                        m_decompressor->feed(std::move(frame->payload));
                        // Read all decompressed data for this single message, enforcing the
                        // configured maximum to guard against compression-bomb payloads.
                        while (m_decompressor->hasData()) {
                            char buffer[4096];
                            qsizetype bytesRead = m_decompressor->read(buffer, sizeof(buffer));
                            if (bytesRead < 0) {
                                finish({ QtGrpc::StatusCode::Internal,
                                         "Decompression failed: %1"_L1
                                             .arg(m_decompressor->errorString()) });
                                return;
                            }
                            // Reject decompressed payloads that would exceed
                            // the configured receive limit.
                            const quint64 newPayloadSize = static_cast<quint64>(finalPayload.size())
                                + static_cast<quint64>(bytesRead);
                            if (newPayloadSize > m_grpcDataParser.maxReceiveSize) {
                                finish({ StatusCode::ResourceExhausted,
                                         QString::asprintf("Decompressed message size (%llu bytes) "
                                                           "exceeded configured limit (%u bytes)",
                                                           newPayloadSize,
                                                           m_grpcDataParser.maxReceiveSize) });
                                return;
                            }
                            finalPayload.append(buffer, bytesRead);
                        }
                        m_decompressor->clear();
                        m_decompressor->setEncoding(m_negotiatedEncoding);
                    } else {
                        finalPayload = std::move(frame->payload);
                    }

                    qCDebug(lcStream,
                            "[%p] Processed gRPC message (compressed=%s, "
                            "payloadSize=%" PRIdQSIZETYPE ", bufferRemaining=%" PRIdQSIZETYPE ")",
                            this, frame->isCompressed ? "true" : "false", finalPayload.size(),
                            m_grpcDataParser.bytesAvailable());

                    emit m_context->messageReceived(finalPayload);
                }

                if (endStream) {
                    if (const auto bytes = m_grpcDataParser.bytesAvailable()) {
                        finish({ QtGrpc::StatusCode::DataLoss,
                                 "Unexpected end of stream with %1 bytes remaining"_L1
                                     .arg(QString::number(bytes)) });
                        return;
                    }
                    finish({});
                }
            });

    // Progressive decrement instead of dequeue-time: bytesToWrite() then
    // includes the unsent remainder of the message in transmission, so it
    // never reports 0 while a large message is stalled mid-window.
    connect(m_stream.get(), &QHttp2Stream::bytesWritten, this, [this](qint64 bytes) {
        Q_ASSERT(quint64(bytes) <= m_bytesToWrite);
        m_bytesToWrite -= quint64(bytes);
        if (m_context)
            m_context->setBytesToWrite(m_bytesToWrite);
    });

    connect(m_stream.get(), &QHttp2Stream::uploadFinished, this, [this] {
        if (std::exchange(m_inFlightUserWrite, false))
            scheduleMessageWritten();

        // sendDATA() may emit uploadFinished synchronously mid-drain; let
        // processQueue()'s loop advance the queue instead of recursing.
        if (m_drainingQueue)
            return;

        processQueue();
    });
}

// Builds HTTP/2 headers for the initial gRPC request.
// The headers are sent once the HTTP/2 connection is established.
HPack::HttpHeader Http2Handler::constructInitialHeaders() const
{
    const static QByteArray AuthorityHeader(":authority");
    const static QByteArray MethodHeader(":method");
    const static QByteArray MethodValue("POST");
    const static QByteArray PathHeader(":path");
    const static QByteArray SchemeHeader(":scheme");

    const static QByteArray TEHeader("te");
    const static QByteArray TEValue("trailers");
    const static QByteArray GrpcServiceNameHeader("service-name");
    const static QByteArray UserAgentHeader("user-agent");
    const static QByteArray UserAgentValue("grpc-c++-qtgrpc/"_ba + QT_VERSION_STR + " ("_ba
                                    + QSysInfo::productType().toUtf8() + '/'
                                    + QSysInfo::productVersion().toUtf8() + ')');

    const auto &channelOptions = channel()->channelOptions();
    auto *channel = channelPriv();

    QByteArray service{ m_context->service() };
    QByteArray method{ m_context->method() };
    auto headers = HPack::HttpHeader{
        { AuthorityHeader,          channel->authorityHeader                 },
        { MethodHeader,             MethodValue                              },
        { PathHeader,               QByteArray('/' + service + '/' + method) },
        { SchemeHeader,             channel->schemeHeader                    },
        { ContentTypeHeader,        channel->contentType                     },
        { GrpcServiceNameHeader,    service                                  },
        { GrpcAcceptEncodingHeader, channel->acceptEncoding()                },
        { UserAgentHeader,          UserAgentValue                           },
        { TEHeader,                 TEValue                                  },
    };
    if (m_requestEncoding != CompressionAlgorithm::Identity)
        headers.emplace_back(GrpcEncodingHeader, requestEncodingName(m_requestEncoding));

    auto iterateMetadata = [&headers, this](const auto &metadata) {
        for (const auto &[key, value] : metadata.asKeyValueRange()) {
            const auto lowerKey = key.toLower();
            // Filter out HTTP/2 and gRPC reserved headers that are managed by the transport.
            if (lowerKey.startsWith(':') || lowerKey.startsWith("grpc-")
                || lowerKey.startsWith("qtgrpc-") || lowerKey == ContentTypeHeader
                || lowerKey == TEHeader || lowerKey == UserAgentHeader) {
                qCWarning(lcStream, "[%p] Dropping user-provided reserved metadata key: '%s'", this,
                          lowerKey.constData());
                continue;
            }
            headers.emplace_back(lowerKey, value);
        }
    };

    iterateMetadata(channelOptions.metadata(QtGrpc::MultiValue));
    iterateMetadata(m_context->callOptions().metadata(QtGrpc::MultiValue));

    return headers;
}

bool Http2Handler::constructFilterServerMetadata() const
{
    return m_context->callOptions()
        .filterServerMetadata()
        .value_or(channel()->channelOptions().filterServerMetadata().value_or(true));
}

CompressionAlgorithm Http2Handler::constructRequestEncoding(const QGrpcHttp2ChannelPrivate &channel,
                                                            const QGrpcOperationContext &context)
{
    return context.callOptions()
        .requestCompression()
        .value_or(channel.q_ptr->channelOptions()
                      .requestCompression()
                      .value_or(CompressionAlgorithm::Identity));
}

QGrpcHttp2ChannelPrivate *Http2Handler::channelPriv() const
{
    return qobject_cast<QGrpcHttp2ChannelPrivate *>(this->parent());
}
QGrpcHttp2Channel *Http2Handler::channel() const
{
    return channelPriv()->q_ptr;
}

bool Http2Handler::handleContextExpired()
{
    if (m_context)
        return false;
    m_state = State::Cancelled;
    deleteLater(); // m_stream will sendRST_STREAM on destruction
    return true;
}

quint32 Http2Handler::constructMaximumReceiveSize(const QGrpcHttp2ChannelPrivate &channel,
                                                  const QGrpcOperationContext &context)
{
    // Call options override channel options.
    if (const auto callVal = context.callOptions().maximumReceiveMessageSize()) {
        return clampToRange(*callVal, 0, GrpcMaxPayloadSize,
                            "QGrpcCallOptions::maximumReceiveMessageSize");
    }
    if (const auto chVal = channel.q_ptr->channelOptions().maximumReceiveMessageSize()) {
        return clampToRange(*chVal, 0, GrpcMaxPayloadSize,
                            "QGrpcChannelOptions::maximumReceiveMessageSize");
    }
    // Environment variable acts as a global fallback when no C++ option is set;
    // applications can always override it in code. Cached on channel construction.
    if (const auto envVal = channel.envMaximumReceiveSize) {
        return clampToRange(*envVal, 0, GrpcMaxPayloadSize, EnvMaximumReceiveMessageSize);
    }
    return DefaultMaximumReceiveMessageSize;
}

// Slot to enqueue a writeMessage request, either from the initial message
// or from the user in client/bidirectional streaming RPCs.
void Http2Handler::writeMessage(QByteArrayView data)
{
    // Common HTTP/gRPC default; below ~30B, compression overhead outweighs savings.
    constexpr quint8 CompressionMinPayloadBytes = 30;

    if (m_writesDoneSent || m_state > State::Active || isStreamClosedForSending()) {
        qCDebug(lcStream, "[%p] Cannot write message (state=%s, writesDone=%d, streamClosed=%d)",
                this, QDebug::toBytes(m_state).constData(), m_writesDoneSent,
                isStreamClosedForSending());
        return;
    }

    if (q20::cmp_greater(data.size(), GrpcMaxPayloadSize)) {
        finish({ StatusCode::ResourceExhausted,
                 QString::asprintf("Outgoing message size (%lld bytes) exceeds "
                                   "configured limit (%u bytes)",
                                   static_cast<qint64>(data.size()), GrpcMaxPayloadSize) });
        return;
    }

    quint8 compressedFlag = 0;
    QByteArray msg;
    qsizetype payloadSize = data.size();

    if (m_requestEncoding != CompressionAlgorithm::Identity
        && payloadSize >= CompressionMinPayloadBytes) {
        if (!m_compressor)
            m_compressor = GrpcCompressionHelper::create(m_requestEncoding);
        if (m_compressor) {
            const auto res = m_compressor->tryCompressMessage(data, msg);
            if (res) {
                payloadSize = *res;
                compressedFlag = GrpcCompressedFlag;
            } else if (res.error() == GrpcCompressionHelper::Error::Broken) {
                // Stream is in an undefined state; discard so the next attempt
                // re-initializes from scratch.
                m_compressor.reset();
            }
        }
    }

    if (compressedFlag == 0) {
        msg = QByteArray(GrpcMessageSizeHeaderSize + data.size(), Qt::Uninitialized);
        if (!data.isEmpty()) {
            std::memcpy(msg.data() + GrpcMessageSizeHeaderSize, data.data(),
                        static_cast<size_t>(data.size()));
        }
    }

    msg[0] = static_cast<char>(compressedFlag);
    qToBigEndian(static_cast<quint32>(payloadSize), msg.data() + 1);

    m_queuedBytes += msg.size();
    m_bytesToWrite += msg.size();
    if (m_context)
        m_context->setBytesToWrite(m_bytesToWrite);

    m_queue.enqueue(std::move(msg));
    processQueue();

    // Checked after processQueue() so a message already handed to the
    // transport never counts against the threshold.
    if (m_queuedBytes > m_nextQueueWarnBytes) {
        qCWarning(lcStream,
                  "[%p] Outgoing message queue exceeds %lld bytes (%lld queued); messages are "
                  "written faster than the connection transmits them.",
                  this, static_cast<qint64>(m_nextQueueWarnBytes),
                  static_cast<qint64>(m_queuedBytes));
        // Escalate the threshold so a steady large upload warns once, while a
        // runaway queue leaves a doubling trail instead of per-write spam.
        while (m_queuedBytes > m_nextQueueWarnBytes)
            m_nextQueueWarnBytes *= 2;
    }
}

// Sends the initial headers and processes the message queue containing the
// initial message. At this point, the HTTP/2 connection is established, and
// the stream is attached.
void Http2Handler::sendInitialRequest()
{
    Q_ASSERT(!m_initialHeaders.empty());
    Q_ASSERT(m_stream);
    if (m_state >= State::Cancelled) {
        qCDebug(lcStream, "[%p] Stream finished before sending the initial request", this);
        return;
    }
    Q_ASSERT(m_state == State::Idle);

    if (!m_stream->sendHEADERS(m_initialHeaders, false)) {
        asyncFinish({ StatusCode::Unavailable,
                      tr("Unable to send initial headers to an HTTP/2 stream") });
        return;
    }
    m_state = State::RequestHeadersSent;
    m_initialHeaders.clear();
    processQueue();

    std::optional<std::chrono::milliseconds> deadline = m_context->callOptions().deadlineTimeout();
    if (!deadline)
        deadline = channel()->channelOptions().deadlineTimeout();
    if (deadline) {
        // We have an active stream, a deadline and the initial headers have
        // just been sent. It's time to start the timer.
        connect(&m_deadlineTimer, &QTimer::timeout, this, &Http2Handler::deadlineTimeout);
        m_deadlineTimer.start(*deadline);
    }
    qCDebug(lcStream, "[%p] Sending initial request (deadline=%s)", this,
            deadline ? qPrintable(QString::number(deadline->count()) + " ms"_L1) : "None");
}

// The core logic for sending the already serialized data through the HTTP/2 stream.
// This function is invoked either by the user via writeMessageRequested() or
// writesDoneRequested(), or it is continuously polled after the previous uploadFinished()
void Http2Handler::processQueue()
{
    if (!m_stream)
        return;

    // HTTP/2 forbids DATA before HEADERS; sendInitialRequest() resumes the
    // drain once the request HEADERS have been sent.
    if (m_state == State::Idle)
        return;

    if (m_stream->isUploadingDATA()) {
        qCDebug(lcStream, "[%p] Stream busy uploading (queue size=%" PRIdQSIZETYPE ")", this,
                m_queue.size());
        return;
    }

    if (m_queue.isEmpty())
        return;

    QScopedValueRollback drainingGuard(m_drainingQueue, true);

    do {
        const auto nextMessage = m_queue.dequeue();
        Q_ASSERT(quint64(nextMessage.size()) <= m_queuedBytes);
        m_queuedBytes -= quint64(nextMessage.size());
        const bool closeStream = nextMessage.isEmpty() || m_endStreamAtFirstData;
        // Set before sendDATA(): a message that fits the send window emits
        // uploadFinished synchronously, and its handler consumes the flag.
        m_inFlightUserWrite = !closeStream;
        if (!m_stream->sendDATA(nextMessage, closeStream)) {
            m_inFlightUserWrite = false;
            return;
        }
    } while (!m_stream->isUploadingDATA() && !m_queue.isEmpty());
}

// Emits messageWritten() deferred to the next event-loop turn: a synchronous
// emission would re-enter the user's slot while writeMessage() is still on
// the call stack. Only the operation ending suppresses delivery, so that the
// signal never follows finished; writesDone() does not invalidate writes
// that already completed.
void Http2Handler::scheduleMessageWritten()
{
    QMetaObject::invokeMethod(
        this,
        [this] {
            if (m_context && m_state <= State::Active)
                emit m_context->messageWritten();
        },
        Qt::QueuedConnection);
}

void Http2Handler::finish(const QGrpcStatus &status)
{
    if (handleContextExpired())
        return;
    if (m_state == State::Finished)
        return;
    if (m_state != State::Cancelled) // don't overwrite the Cancelled state
        m_state = State::Finished;
    m_deadlineTimer.stop();
    emit m_context->finished(status);
    deleteLater();
}
void Http2Handler::asyncFinish(const QGrpcStatus &status)
{
    if (handleContextExpired())
        return;
    QTimer::singleShot(0, m_context.get(), [this, status]() { finish(status); });
}

void Http2Handler::cancelWithStatus(const QGrpcStatus &status)
{
    if (m_state >= State::Cancelled) {
        qCWarning(lcStream, "[%p] Cannot cancel stream in state=%s", this,
                  QDebug::toBytes(m_state).constData());
        return;
    }
    qCDebug(lcStream, "[%p] Cancelling (state=%s)", this, QDebug::toBytes(m_state).constData());
    m_state = State::Cancelled;

    if (m_stream && m_stream->state() != QHttp2Stream::State::Idle) {
        // Immediate cancellation by sending the RST_STREAM frame.
        if (!m_stream->sendRST_STREAM(Http2::Http2Error::CANCEL)) {
            qCWarning(lcStream, "[%p] Failed cancellation (stream=%p, stream::state=%s)", this,
                      m_stream.get(), QDebug::toBytes(m_stream->state()).constData());
        }
    }

    finish(status);
}

void Http2Handler::writesDone()
{
    if (m_writesDoneSent || m_state > State::Active)
        return;
    m_writesDoneSent = true;

    qCDebug(lcStream, "[%p] Writes done received (streamClosed=%d)", this, isStreamClosedForSending());

    // Stream is already (half)closed, skip sending the DATA frame with the end-of-stream flag.
    if (isStreamClosedForSending())
        return;

    m_queue.enqueue({});
    processQueue();
}

void Http2Handler::handleHeaders(const HPack::HttpHeader &headers, HeaderPhase phase)
{
    // ABNF syntax: Rule, [Optional-Rule], *Variable-Repetition
    // Response-Headers → HTTPStatus [GrpcEncoding] [GrpcAcceptEncoding]
    //                    ContentType *Custom-Metadata
    // Trailers      → GrpcStatus [GrpcStatusMessage] [GrpcStatusDetails] *Custom-Metadata
    // Trailers-Only → HTTPStatus ContentType Trailers
    //
    // It's either Response-Headers + Trailers OR Trailers-Only for calls that
    // produce an immediate error. Any Trailers phase will finish the RPC.
    Q_ASSERT(phase != HeaderPhase::Invalid);
    struct HeaderValidation
    {
        const bool requireHttpStatus : 1;
        const bool requireContentType : 1;
        const bool requireGrpcStatus : 1;
        bool hasHttpStatus : 1;
        bool hasContentType : 1;
        bool hasGrpcStatus : 1;
    };

    if (handleContextExpired())
        return;

    HeaderValidation validation{
        (phase != HeaderPhase::Trailers),
        (phase != HeaderPhase::Trailers),
        (phase != HeaderPhase::Initial),
        false,
        false,
        false,
    };

    QMultiHash<QByteArray, QByteArray> metadata;
    std::optional<QtGrpc::StatusCode> statusCode;
    QString statusMessage;

    for (const auto &[k, v] : headers) {
        if (validation.requireHttpStatus && k == HttpStatusHeader) {
            if (const auto status = v.toInt(); status != 200) {
                finish({ http2StatusToStatusCode(status), "Received HTTP/2 status: %1"_L1.arg(v) });
                return;
            }
            validation.hasHttpStatus = true;
        } else if (validation.requireContentType && k == ContentTypeHeader) {
            if (!v.toLower().startsWith(DefaultContentType)) {
                finish({ StatusCode::Internal, "Unexpected content-type: %1"_L1.arg(v) });
                return;
            }
            validation.hasContentType = true;
        } else if (validation.requireGrpcStatus && k == GrpcStatusHeader) {
            bool ok;
            const auto parsed = v.toShort(&ok);
            if (!ok || parsed < 0 || parsed > QtGrpcPrivate::StatusCodeCount) {
                finish({ StatusCode::Unknown, "Received invalid gRPC-status: %1"_L1.arg(v) });
                return;
            }
            statusCode = static_cast<StatusCode>(parsed);
            validation.hasGrpcStatus = true;
        } else if (validation.requireGrpcStatus && k == GrpcStatusMessageHeader) {
            // Allowed optional headers
            statusMessage = QString::fromUtf8(v);
        } else if (validation.requireGrpcStatus && k == GrpcStatusDetailsHeader) {
            // Allowed optional headers
            // TODO: Implement status-details - QTBUG-138362
        } else if (phase == HeaderPhase::Initial && k == GrpcEncodingHeader) {
            // Allowed optional header. Per the gRPC compression spec, an
            // unsupported server-side encoding MUST be reported as Internal.
            const auto accepted = channel()->channelOptions().acceptedCompressionAlgorithms();
            if (!isAcceptedEncoding(v, accepted)) {
                finish({ StatusCode::Internal,
                         "Server responded with an encoding not advertised by client: %1"_L1
                             .arg(v) });
                return;
            }
            if (v == "identity"_ba) {
                m_negotiatedEncoding.clear();
            } else {
                m_decompressor = std::make_unique<QDecompressHelper>();
                if (!m_decompressor->setEncoding(v)) {
                    finish({ StatusCode::Internal,
                             "Failed to initialize decompressor for algorithm: %1"_L1.arg(v) });
                    return;
                }
                m_negotiatedEncoding = v;
            }
        } else if (phase == HeaderPhase::Initial && k == GrpcAcceptEncodingHeader) {
            // The server advertises what it can decode. The client does not filter
            // its own request compression against this set; a mismatch causes the
            // server to respond with UNIMPLEMENTED (per the gRPC compression spec).
            if (m_requestEncoding != CompressionAlgorithm::Identity) {
                const QByteArray name = requestEncodingName(m_requestEncoding);
                const auto tokens = v.split(',');
                const bool advertised = std::any_of(tokens.cbegin(), tokens.cend(),
                                                    [&name](const QByteArray &t) {
                                                        return t.trimmed() == name;
                                                    });
                if (!advertised) {
                    qCWarning(lcStream,
                              "[%p] Outbound request-compression '%s' is not in the "
                              "server's grpc-accept-encoding ('%s'); the server is "
                              "expected to respond with UNIMPLEMENTED.",
                              this, name.constData(), sanitizedForLog(v).constData());
                }
            }
        } else if (k.startsWith(':')) {
            qCWarning(lcStream,
                      "[%p] Received unhandled HTTP/2 pseudo-header: { key: '%s', value: '%s' } "
                      "in phase: %s",
                      this, sanitizedForLog(k).constData(), sanitizedForLog(v).constData(),
                      QDebug::toBytes(phase).constData());
        } else if (k.startsWith("grpc-")) {
            qCWarning(lcStream,
                      "[%p] Received unexpected gRPC-reserved header: { key: %s, value: %s } "
                      "in phase: %s",
                      this, sanitizedForLog(k).constData(), sanitizedForLog(v).constData(),
                      QDebug::toBytes(phase).constData());
        } else { // Custom-Metadata
            metadata.insert(k, v);
            continue;
        }

        if (!m_filterServerMetadata)
            metadata.insert(k, v);
    }

    if (validation.requireHttpStatus && !validation.hasHttpStatus) {
        finish({ StatusCode::Internal, "Missing valid '%1' header"_L1.arg(HttpStatusHeader) });
        return;
    }

    if (validation.requireContentType && !validation.hasContentType) {
        finish({ StatusCode::Internal, "Missing valid '%1' header"_L1.arg(ContentTypeHeader) });
        return;
    }

    if (validation.requireGrpcStatus && !validation.hasGrpcStatus) {
        finish({ StatusCode::Internal, "Missing status code in trailers"_L1 });
        return;
    }

    switch (phase) {
    case HeaderPhase::Initial:
        m_context->setServerInitialMetadata(std::move(metadata));
        break;
    case HeaderPhase::TrailersOnly:
        [[fallthrough]];
    case HeaderPhase::Trailers:
        m_context->setServerTrailingMetadata(std::move(metadata));
        finish({ *statusCode, statusMessage });
        break;
    default:
        Q_UNREACHABLE();
    }
}

///
/// ## QGrpcHttp2ChannelPrivate Implementations
///

QGrpcHttp2ChannelPrivate::QGrpcHttp2ChannelPrivate(const QUrl &uri, QGrpcHttp2Channel *q)
    : q_ptr(q), socketType(constructSocketType(uri, q_ptr->channelOptions())),
      hostUri(sanitizeHostUri(uri, q_ptr->channelOptions())),
      contentType(setupContentTypeNegotiation(q_ptr)),
      authorityHeader(constructAuthorityHeader(hostUri, socketType)),
      schemeHeader(constructSchemeHeader(socketType)),
      m_reconnectBackoff(constructReconnectBackoff(q_ptr->channelOptions(),
                                                   envInitialReconnectBackoff,
                                                   envMaximumReconnectBackoff))
{
    switch (socketType) {
    case SocketType::Tcp: {
        auto socket = std::make_unique<QTcpSocket>();
        connect(socket.get(), &QAbstractSocket::connected, this,
                &QGrpcHttp2ChannelPrivate::createHttp2Connection);
        connect(socket.get(), &QAbstractSocket::errorOccurred, this,
                &QGrpcHttp2ChannelPrivate::handleAbstractSocketError);
        m_reconnectFunction = [this, socket = socket.get()] {
            qCDebug(lcChannel, "[%p] Connecting to TCP endpoint at: %s:%d", this,
                    qPrintable(hostUri.host()), hostUri.port());
            socket->connectToHost(hostUri.host(), hostUri.port());
        };
        m_socket = std::move(socket);
        break;
    }

    case SocketType::Tls: {
#if QT_CONFIG(ssl)
        auto socket = std::make_unique<QSslSocket>();
        if (const auto &sslConfig = q_ptr->channelOptions().sslConfiguration()) {
            socket->setSslConfiguration(*sslConfig);
        } else {
            static const QByteArray h2NexProtocol = "h2"_ba;
            auto defaultSslConfig = QSslConfiguration::defaultConfiguration();
            auto allowedNextProtocols = defaultSslConfig.allowedNextProtocols();
            if (!allowedNextProtocols.contains(h2NexProtocol)) {
                allowedNextProtocols.append(h2NexProtocol);
                defaultSslConfig.setAllowedNextProtocols(allowedNextProtocols);
            }
            socket->setSslConfiguration(defaultSslConfig);
        }
        connect(socket.get(), &QSslSocket::encrypted, this,
                &QGrpcHttp2ChannelPrivate::createHttp2Connection);
        connect(socket.get(), &QAbstractSocket::errorOccurred, this,
                &QGrpcHttp2ChannelPrivate::handleAbstractSocketError);
        m_reconnectFunction = [this, socket = socket.get()] {
            qCDebug(lcChannel, "[%p] Connecting to SSL endpoint at: %s:%d", this,
                    qPrintable(hostUri.host()), hostUri.port());
            socket->connectToHostEncrypted(hostUri.host(), hostUri.port());
        };
        m_socket = std::move(socket);
#else
        m_reconnectFunction = [this] {
            qCFatal(lcChannel, "[%p] QSslSocket support needed for TLS transportation", this);
        };
#endif
        break;
    }

    case SocketType::Local:
    case SocketType::LocalAbstract: {
#if QT_CONFIG(localserver)
        auto socket = std::make_unique<QLocalSocket>();
        if (socketType == SocketType::LocalAbstract)
            socket->setSocketOptions(QLocalSocket::AbstractNamespaceOption);
        connect(socket.get(), &QLocalSocket::connected, this,
                &QGrpcHttp2ChannelPrivate::createHttp2Connection);
        connect(socket.get(), &QLocalSocket::errorOccurred, this,
                &QGrpcHttp2ChannelPrivate::handleLocalSocketError);
        m_reconnectFunction = [this, socket = socket.get()] {
            const QString name = hostUri.host() + hostUri.path();
            qCDebug(lcChannel, "[%p] Connecting to local socket at: %s", this, qPrintable(name));
            socket->connectToServer(name);
        };
        m_socket = std::move(socket);
#else
        m_reconnectFunction = [this] {
            qCFatal(lcChannel,
                    "[%p] QLocalSocket support needed for 'unix' or 'unix-abstract' transportation",
                    this);
        };
#endif
        break;
    }

    } // switch (socketType)

    m_reconnectTimer.setSingleShot(true);
    m_reconnectTimer.callOnTimeout(this, &QGrpcHttp2ChannelPrivate::attemptConnect);

    m_connectTimeoutTimer.setSingleShot(true);
    m_connectTimeoutTimer
        .setInterval(resolveMs(QGrpcChannelOptionsPrivate::get(q_ptr->channelOptions())
                                   ->connectTimeout,
                               envConnectTimeout, QtGrpcPrivate::DefaultConnectTimeout));
    m_connectTimeoutTimer.callOnTimeout(this, [this] {
        qCWarning(lcChannel, "[%p] Connection attempt timed out after %dms; aborting.", this,
                  m_connectTimeoutTimer.interval());
        visitSocket([](auto *socket) { socket->abort(); });
        handleSocketError("ConnectTimeout"_ba);
    });

    attemptConnect();
}

QGrpcHttp2ChannelPrivate::~QGrpcHttp2ChannelPrivate()
{
    for_each_non_expired_handler([](Http2Handler *handler) {
        emit handler->finish({ StatusCode::Unavailable, tr("Channel destroyed") });
    });

    if (!m_socket)
        return;
    // The teardown below can emit errorOccurred synchronously; detach our
    // handlers so none of them runs on a channel that is mid-destruction.
    m_socket->disconnect(this);
    // m_connection is parented to m_socket; delete it while the socket is
    // still intact so destroying active streams can send their RST_STREAM.
    delete std::exchange(m_connection, nullptr);
    // The socket destructor discards unsent data; flush the RST_STREAM out.
    visitSocket([](auto *socket) { socket->flush(); });
}

void QGrpcHttp2ChannelPrivate::processOperation(QGrpcOperationContext *operationContext,
                                                QByteArray &&messageData, bool endStream)
{
    Q_ASSERT_X(operationContext != nullptr, "QGrpcHttp2ChannelPrivate::processOperation",
               "operation context is nullptr.");

    // Send the finished signals asynchronously, so user connections work correctly.
    if (!m_socket->isWritable() && m_state == ConnectionState::Connected) {
        qCWarning(lcChannel, "[%p] Socket not writable for operation to %s (error=%s)", this,
                  qPrintable(hostUri.toString()), qPrintable(m_socket->errorString()));
        QTimer::singleShot(0, operationContext,
                           [operationContext, err = m_socket->errorString()]() {
                               emit operationContext->finished({ StatusCode::Unavailable, err });
                           });
        return;
    }

    auto *handler = new Http2Handler(this, operationContext, std::move(messageData), endStream);
    if (m_connection && !createHttp2Stream(handler))
        return;

    if (m_state == ConnectionState::SettingsReceived)
        handler->sendInitialRequest();

    if (m_state == ConnectionState::Error) {
        Q_ASSERT_X(m_reconnectFunction, "QGrpcHttp2ChannelPrivate::processOperation",
                   "Socket reconnection function is not defined.");
        if (!m_reconnectTimer.isActive()) {
            const auto delay = m_reconnectBackoff.nextDelay();
            qCDebug(lcChannel, "[%p] Scheduling reconnect in %lldms.", this,
                    static_cast<long long>(delay.count()));
            m_reconnectTimer.start(delay);
        }
        m_state = ConnectionState::Connecting;
        qCDebug(lcChannel, "[%p] State changed to 'Connecting'. Reconnection initiated.", this);
    }
}

void QGrpcHttp2ChannelPrivate::attemptConnect()
{
    Q_ASSERT_X(m_reconnectFunction, "QGrpcHttp2ChannelPrivate::attemptConnect",
               "Socket reconnection function is not defined.");
    if (m_connectTimeoutTimer.interval() > 0)
        m_connectTimeoutTimer.start();
    m_reconnectFunction();
}

void QGrpcHttp2ChannelPrivate::createHttp2Connection()
{
    Q_ASSERT_X(m_connection == nullptr, "QGrpcHttp2ChannelPrivate::createHttp2Connection",
               "Attempt to create the HTTP/2 connection, but it already exists. This situation is "
               "exceptional.");

    // Nagle's algorithm slows down gRPC communication when frequently sending small utility
    // HTTP/2 frames. Since an ACK is not sent until a predefined timeout if the TCP frame is
    // not full enough, communication hangs. In our case, this results in a 40ms delay when
    // WINDOW_UPDATE or PING frames are sent in a separate TCP frame.
    //
    // TODO: We should probably allow users to opt out of this using QGrpcChannelOptions,
    // see QTBUG-134428.
    if (QAbstractSocket *abstractSocket = qobject_cast<QAbstractSocket *>(m_socket.get()))
        abstractSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    m_connection = QHttp2Connection::createDirectConnection(m_socket.get(),
                                                            createHttp2Configuration(*this));

    Q_ASSERT_X(m_connection, "QGrpcHttp2ChannelPrivate", "Unable to create the HTTP/2 connection");
    connect(m_socket.get(), &QAbstractSocket::readyRead, m_connection,
            &QHttp2Connection::handleReadyRead);

    m_state = ConnectionState::Connected;
    qCDebug(lcChannel, "[%p] Created new HTTP/2 connection to %s", this,
            qPrintable(hostUri.toString()));

    connect(m_connection, &QHttp2Connection::settingsFrameReceived, this, [this] {
        if (m_state == ConnectionState::SettingsReceived)
            return;
        // The gRPC connection-backoff spec recommends a backoff reset when the SETTINGS
        // frame is received, confirming the server fully accepted the connection.
        m_connectTimeoutTimer.stop();
        m_reconnectBackoff.reset();
        m_state = ConnectionState::SettingsReceived;
        qCDebug(lcChannel, "[%p] SETTINGS frame received. Connection ready for use.", this);
        for_each_non_expired_handler([](Http2Handler *handler) { handler->sendInitialRequest(); });
    });

    for_each_non_expired_handler([this](Http2Handler *handler) { createHttp2Stream(handler); });
}

void QGrpcHttp2ChannelPrivate::handleSocketError(const QByteArray &errorCode)
{
    for_each_non_expired_handler([this, &errorCode](Http2Handler *handler) {
        if (m_isInsideSocketErrorOccurred) {
            qCCritical(lcChannel,
                        "[%p] Socket errorOccurred signal triggered while "
                        "already handling an error",
                        this);
            return;
        }
        m_isInsideSocketErrorOccurred = true;
        auto reset = qScopeGuard([this]() { m_isInsideSocketErrorOccurred = false; });
        emit handler->finish({ StatusCode::Unavailable,
                                tr("Network error occurred: %1").arg(errorCode) });
    });

    m_connectTimeoutTimer.stop();
    qCDebug(lcChannel, "[%p] Socket error occurred (code=%s, details=%s, hostUri=%s)", this,
            errorCode.constData(), qPrintable(m_socket->errorString()),
            qPrintable(hostUri.toString()));
    delete std::exchange(m_connection, nullptr);
    m_state = ConnectionState::Error;
}

QUrl QGrpcHttp2ChannelPrivate::sanitizeHostUri(const QUrl &rawUri,
                                               const QGrpcChannelOptions &chOpts) const
{
    QUrl sanitizedUri(rawUri);
    auto check = [&](QLatin1StringView expected) {
        if (rawUri.scheme() != expected) {
            qCWarning(lcChannel,
                      "[%p] Unsupported transport protocol scheme '%s'. Fall back to '%s'.", this,
                      qPrintable(rawUri.scheme()), qPrintable(expected));
            sanitizedUri.setScheme(expected);
        }
    };
    const auto scheme = rawUri.scheme();
    if (scheme == UnixScheme || scheme == UnixAbstractScheme) {
        // Keep the unix/unix-abstract scheme so the sanitized URI is
        // idempotent: re-feeding it into QGrpcHttp2Channel must select the
        // same SocketType.
    } else if (scheme == HttpsScheme || hasSslConfiguration(chOpts)) {
        check(HttpsScheme);
        if (rawUri.port() < 0)
            sanitizedUri.setPort(443);
    } else {
        check(HttpScheme);
        if (rawUri.port() < 0)
            sanitizedUri.setPort(80);
    }
    return sanitizedUri;
}

QGrpcHttp2ChannelPrivate::SocketType
QGrpcHttp2ChannelPrivate::constructSocketType(const QUrl &rawUri, const QGrpcChannelOptions &chOpts)
{
    const auto scheme = rawUri.scheme();
    if (scheme == UnixScheme)
        return SocketType::Local;
    if (scheme == UnixAbstractScheme)
        return SocketType::LocalAbstract;
    if (scheme == HttpsScheme || hasSslConfiguration(chOpts))
        return SocketType::Tls;
    return SocketType::Tcp;
}

QByteArray QGrpcHttp2ChannelPrivate::setupContentTypeNegotiation(QGrpcHttp2Channel *qPtr) const
{
    auto channelOptions = qPtr->channelOptions();
    auto formatSuffix = channelOptions.serializationFormat().suffix();
    const QByteArray defaultContentType = DefaultContentType;
    const QByteArray contentTypeFromOptions = !formatSuffix.isEmpty()
        ? defaultContentType + '+' + formatSuffix
        : defaultContentType;

    bool warnAboutFormatConflict = !formatSuffix.isEmpty();
    QByteArray finalContentType = contentTypeFromOptions;

    const auto it = channelOptions.metadata(QtGrpc::MultiValue).constFind(ContentTypeHeader.data());
    if (it != channelOptions.metadata(QtGrpc::MultiValue).cend()) {
        if (formatSuffix.isEmpty() && it.value() != DefaultContentType) {
            // Auto-detect format from content-type header
            if (it.value() == "application/grpc+json") {
                channelOptions.setSerializationFormat(SerializationFormat::Json);
            } else if (it.value() == "application/grpc+proto" || it.value() == DefaultContentType) {
                channelOptions.setSerializationFormat(SerializationFormat::Protobuf);
            } else {
                qCWarning(lcChannel,
                          "[%p] Unable to determine serializer for entry { key: %s, value: %s }. "
                          "Defaulting to format '%s'",
                          this, it.key().data(), it.value().data(),
                          QDebug::toBytes(SerializationFormat::Default).constData());
                channelOptions.setSerializationFormat(SerializationFormat::Default);
            }
            qPtr->setChannelOptions(channelOptions);
            warnAboutFormatConflict = false;
        } else if (it.value() != contentTypeFromOptions) {
            warnAboutFormatConflict = true;
        } else {
            warnAboutFormatConflict = false;
        }
    } else {
        warnAboutFormatConflict = false;
    }

    // Update final content type if format changed
    if (formatSuffix != channelOptions.serializationFormat().suffix()) {
        finalContentType = !channelOptions.serializationFormat().suffix().isEmpty()
            ? defaultContentType + '+' + channelOptions.serializationFormat().suffix()
            : defaultContentType;
    }

    if (warnAboutFormatConflict) {
        qCWarning(lcChannel,
                  "[%p] Manually specified serialization format '%s' does not "
                  "match metadata entry { key: %s, value: %s }",
                  this, contentTypeFromOptions.data(), it.key().data(), it.value().data());
    }

    return finalContentType;
}

QByteArray QGrpcHttp2ChannelPrivate::constructAuthorityHeader(const QUrl &hostUri,
                                                              SocketType socketType)
{
    auto authority = hostUri.authority(QUrl::FullyEncoded | QUrl::RemoveUserInfo | QUrl::RemovePort)
                         .toLatin1();
    const int port = hostUri.port();
    if ((socketType == SocketType::Tcp && port != 80)
        || (socketType == SocketType::Tls && port != 443)) {
        authority += ':';
        authority += QByteArray::number(port);
    }

    return authority;
}

QByteArray QGrpcHttp2ChannelPrivate::constructSchemeHeader(SocketType socketType)
{
    return socketType == SocketType::Tls ? "https"_ba : "http"_ba;
}

ReconnectBackoff
QGrpcHttp2ChannelPrivate::constructReconnectBackoff(const QGrpcChannelOptions &chOpts,
                                                    std::optional<quint64> envInitial,
                                                    std::optional<quint64> envMaximum)
{
    const auto *chOptsPriv = QGrpcChannelOptionsPrivate::get(chOpts);
    auto effectiveInitial = resolveMs(chOptsPriv->initialReconnectBackoff, envInitial,
                                      QtGrpcPrivate::DefaultInitialReconnectBackoff);
    const auto effectiveMaximum = resolveMs(chOptsPriv->maximumReconnectBackoff, envMaximum,
                                            QtGrpcPrivate::DefaultMaximumReconnectBackoff);
    if (effectiveInitial > effectiveMaximum) {
        qCWarning(lcChannel,
                  "Initial reconnect backoff of %lldms exceeds the maximum of %lldms; "
                  "clamping to the maximum.",
                  static_cast<qint64>(effectiveInitial.count()),
                  static_cast<qint64>(effectiveMaximum.count()));
        effectiveInitial = effectiveMaximum;
    }
    return { effectiveInitial, effectiveMaximum };
}

QByteArray QGrpcHttp2ChannelPrivate::constructAcceptEncoding(QtGrpc::CompressionAlgorithms flags)
{
    // "identity" (uncompressed) is mandated by the gRPC specification and is
    // therefore always advertised.
    QByteArray result = "identity";

    if (flags & CompressionAlgorithm::Deflate)
        result += ",deflate";
    if (flags & CompressionAlgorithm::Gzip)
        result += ",gzip";
    return result;
}

auto QGrpcHttp2ChannelPrivate::constructReceiveWindowSizes() -> ReceiveWindowSizes
{
    // When the connection window is left to the default it is sized as this
    // many per-stream windows.
    constexpr quint32 ConnectionWindowStreams = 4;

    // Resolve one window: take the env override (or the default) and clamp it
    // into [floor, maxSessionReceiveWindowSize]. The connection window keeps the
    // RFC 9113 (6.9.2) 64 KiB initial window as its floor; the per-stream window
    // allows a smaller floor so constrained receivers can cap per-stream
    // buffering (RFC 6.9.3 permits small per-stream windows).
    const auto resolveWindow = [](const char *env, quint64 dflt, quint32 floor) -> quint32 {
        return clampToRange(readEnvUnsignedInt(env).value_or(dflt), floor,
                            Http2::maxSessionReceiveWindowSize, env);
    };

    quint32 streamWindow = resolveWindow(EnvHttp2StreamReceiveWindowSize,
                                         DefaultHttp2StreamReceiveWindowSize,
                                         MinimumHttp2StreamReceiveWindowSize);
    // Pre-clamp the derived default so a stream window near either bound
    // doesn't trigger a warning blaming an environment variable the user
    // never set.
    const quint64 connectionDefault = qBound(quint64(Http2::defaultSessionWindowSize),
                                             quint64(streamWindow) * ConnectionWindowStreams,
                                             quint64(Http2::maxSessionReceiveWindowSize));
    const quint32 connectionWindow = resolveWindow(EnvHttp2ConnReceiveWindowSize, connectionDefault,
                                                   Http2::defaultSessionWindowSize);

    // Flow control caps in-flight data per stream at min(stream, connection)
    // anyway; clamping keeps the advertised windows equal to the effective
    // behavior while respecting the connection window as the memory budget.
    if (connectionWindow < streamWindow) {
        qCWarning(lcChannel,
                  "Connection receive-window of %u bytes (%s) is smaller than the stream "
                  "receive-window of %u bytes; clamping the stream window to the connection "
                  "window.",
                  connectionWindow, EnvHttp2ConnReceiveWindowSize, streamWindow);
        streamWindow = connectionWindow;
    }

    return { streamWindow, connectionWindow };
}

QHttp2Configuration
QGrpcHttp2ChannelPrivate::createHttp2Configuration(const QGrpcHttp2ChannelPrivate &channel)
{
    QHttp2Configuration config;
    // These setters are expected to succeed. If they fail, the default HTTP/2
    // receive-window sizes remain in effect.
    const auto windows = constructReceiveWindowSizes();
    if (!config.setStreamReceiveWindowSize(windows.stream)) {
        qCWarning(lcChannel, "Stream receive-window of '%u' rejected by QHttp2Configuration",
                  windows.stream);
    }
    if (!config.setSessionReceiveWindowSize(windows.connection)) {
        qCWarning(lcChannel, "Session receive-window of '%u' rejected by QHttp2Configuration",
                  windows.connection);
    }

    // Advertise the metadata limit as SETTINGS_MAX_HEADER_LIST_SIZE;
    // QHttp2Connection also enforces it on every received header block.
    // The environment variable is a global fallback used only while the option
    // is not explicitly set.
    constexpr quint32 Cap = (std::numeric_limits<quint32>::max)();
    quint64 requested = QtGrpcPrivate::DefaultMaximumMetadataSize;
    const char *source = "QGrpcChannelOptions::maximumMetadataSize";
    if (const auto chVal = QGrpcChannelOptionsPrivate::get(channel.q_ptr->channelOptions())
                               ->maximumMetadataSize) {
        requested = *chVal;
    } else if (const auto envVal = channel.envMaximumMetadataSize) {
        requested = *envVal;
        source = EnvGrpcMaximumMetadataSize;
    }
    config.setMaxHeaderListSize(clampToRange(requested, QtGrpcPrivate::MinimumMetadataSize, Cap,
                                             source));

    return config;
}

const QByteArray &QGrpcHttp2ChannelPrivate::acceptEncoding()
{
    const auto flags = q_ptr->channelOptions().acceptedCompressionAlgorithms();
    if (m_acceptEncoding.isNull() || flags != m_acceptEncodingFlags) {
        m_acceptEncoding = constructAcceptEncoding(flags);
        m_acceptEncodingFlags = flags;
    }
    return m_acceptEncoding;
}

bool QGrpcHttp2ChannelPrivate::createHttp2Stream(Http2Handler *handler)
{
    Q_ASSERT(handler != nullptr);
    Q_ASSERT(m_connection);
    constexpr QHttp2Stream::Configuration StreamConfiguration = {
        false, // useDownloadBuffer
    };
    const auto streamAttempt = m_connection->createStream(StreamConfiguration);
    if (!streamAttempt.ok()) {
        handler->asyncFinish({ StatusCode::Unavailable,
                               tr("Unable to create an HTTP/2 stream (%1)")
                                   .arg(QDebug::toString(streamAttempt.error())) });
        return false;
    }
    handler->attachStream(streamAttempt.unwrap());
    return true;
}

///
/// ## QGrpcHttp2Channel Implementations
///

/*!
    Constructs QGrpcHttp2Channel with \a hostUri. Please see the
    \l{Transportation scheme} section for more information.
*/
QGrpcHttp2Channel::QGrpcHttp2Channel(const QUrl &hostUri)
    : d_ptr(std::make_unique<QGrpcHttp2ChannelPrivate>(hostUri, this))
{
}

/*!
    Constructs QGrpcHttp2Channel with \a hostUri and \a options. Please see the
    \l{Transportation scheme} section for more information.
*/
QGrpcHttp2Channel::QGrpcHttp2Channel(const QUrl &hostUri, const QGrpcChannelOptions &options)
    : QAbstractGrpcChannel(options),
      d_ptr(std::make_unique<QGrpcHttp2ChannelPrivate>(hostUri, this))
{
}

QGrpcHttp2Channel::QGrpcHttp2Channel(const QUrl &hostUri, QGrpcInterceptorChain interceptorChain)
    : QAbstractGrpcChannel(std::move(interceptorChain)),
      d_ptr(std::make_unique<QGrpcHttp2ChannelPrivate>(hostUri, this))
{
}

QGrpcHttp2Channel::QGrpcHttp2Channel(const QUrl &hostUri, const QGrpcChannelOptions &options,
                                     QGrpcInterceptorChain interceptorChain)
    : QAbstractGrpcChannel(options, std::move(interceptorChain)),
      d_ptr(std::make_unique<QGrpcHttp2ChannelPrivate>(hostUri, this))
{
}

/*!
    Destroys the QGrpcHttp2Channel object.
*/
QGrpcHttp2Channel::~QGrpcHttp2Channel() = default;

/*!
    Returns the host URI for this channel.

    The URI is normalized according to the \l{Transportation scheme}: the
    scheme may be adjusted and a default port may be filled in. Passing the
    returned URI back to QGrpcHttp2Channel will select the same transport
    configuration.
*/
QUrl QGrpcHttp2Channel::hostUri() const
{
    return d_ptr->hostUri;
}

/*!
    \internal
    Initiates a unary \gRPC call.
*/
void QGrpcHttp2Channel::call(QGrpcOperationContext *operationContext, QByteArray &&messageData)
{
    d_ptr->processOperation(operationContext, std::move(messageData), true);
}

/*!
    \internal
    Initiates a server-side \gRPC stream.
*/
void QGrpcHttp2Channel::serverStream(QGrpcOperationContext *operationContext,
                                     QByteArray &&messageData)
{
    d_ptr->processOperation(operationContext, std::move(messageData), true);
}

/*!
    \internal
    Initiates a client-side \gRPC stream.
*/
void QGrpcHttp2Channel::clientStream(QGrpcOperationContext *operationContext,
                                     QByteArray &&messageData)
{
    d_ptr->processOperation(operationContext, std::move(messageData), false);
}

/*!
    \internal
    Initiates a bidirectional \gRPC stream.
*/
void QGrpcHttp2Channel::bidiStream(QGrpcOperationContext *operationContext,
                                   QByteArray &&messageData)
{
    d_ptr->processOperation(operationContext, std::move(messageData), false);
}

/*!
    \internal
    Returns the serializer of the channel.
*/
std::shared_ptr<QAbstractProtobufSerializer> QGrpcHttp2Channel::serializer() const
{
    return channelOptions().serializationFormat().serializer();
}

QT_END_NAMESPACE

#include "qgrpchttp2channel.moc"
