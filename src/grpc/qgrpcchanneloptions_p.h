// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCHANNELOPTIONS_P_H
#define QGRPCCHANNELOPTIONS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtGrpc/private/qgrpccommonoptions_p.h>
#include <QtGrpc/qgrpcchanneloptions.h>
#include <QtGrpc/qgrpcserializationformat.h>

#include <chrono>
#include <optional>

QT_BEGIN_NAMESPACE

namespace QtGrpcPrivate {

constexpr std::chrono::milliseconds DefaultInitialReconnectBackoff = std::chrono::seconds(1);
constexpr std::chrono::milliseconds DefaultMaximumReconnectBackoff = std::chrono::seconds(120);
constexpr std::chrono::milliseconds DefaultConnectTimeout = std::chrono::seconds(20);

} // namespace QtGrpcPrivate

class QGrpcChannelOptionsPrivate : public QGrpcCommonOptions
{
public:
    QGrpcSerializationFormat serializationFormat;
    QtGrpc::CompressionAlgorithms
        acceptedCompressionAlgorithms = QGrpcChannelOptions::supportedCompressionAlgorithms();
    // nullopt: not explicitly set; transports may apply an environment
    // fallback before the QtGrpcPrivate default.
    std::optional<std::chrono::milliseconds> initialReconnectBackoff;
    std::optional<std::chrono::milliseconds> maximumReconnectBackoff;
    std::optional<std::chrono::milliseconds> connectTimeout;
#if QT_CONFIG(ssl)
    std::optional<QSslConfiguration> sslConfiguration;
#endif

    static const QGrpcChannelOptionsPrivate *get(const QGrpcChannelOptions &opts)
    {
        return opts.d_ptr.data();
    }
};

QT_END_NAMESPACE

#endif // QGRPCCHANNELOPTIONS_P_H
