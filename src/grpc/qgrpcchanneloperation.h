// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCCHANNELOPERATION_H
#define QGRPCCHANNELOPERATION_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qbytearrayview.h>
#include <QtCore/qlatin1stringview.h>
#include <QtCore/qobject.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QGrpcCallOptions;
class QGrpcChannelOperationPrivate;
class QGrpcStatus;

class Q_GRPC_EXPORT QGrpcChannelOperation final : public QObject
{
    Q_OBJECT
    struct PrivateConstructor { explicit PrivateConstructor() = default; };
public:
    explicit QGrpcChannelOperation(QLatin1StringView method, QLatin1StringView service,
                                   QByteArrayView argument, const QGrpcCallOptions &options,
                                   std::shared_ptr<QAbstractProtobufSerializer> serializer,
                                   PrivateConstructor);
    ~QGrpcChannelOperation() override;

    [[nodiscard]] QLatin1StringView method() const noexcept;
    [[nodiscard]] QLatin1StringView service() const noexcept;
    [[nodiscard]] QByteArrayView argument() const noexcept;
    [[nodiscard]] const QGrpcCallOptions &callOptions() const & noexcept;
    [[nodiscard]] std::shared_ptr<const QAbstractProtobufSerializer> serializer() const noexcept;

    [[nodiscard]] const QGrpcMetadata &serverMetadata() const & noexcept;

    // Should never happen. Type is not movable.
    QGrpcMetadata serverMetadata() && noexcept = delete;
    QGrpcCallOptions callOptions() && noexcept = delete;

    void setArgument(QByteArrayView arg);
    void setCallOptions(const QGrpcCallOptions &options);

    void setServerMetadata(const QGrpcMetadata &metadata);
    void setServerMetadata(QGrpcMetadata &&metadata);

Q_SIGNALS:
    // Outgoing signals of the channel.
    void finished(const QGrpcStatus &status);
    void messageReceived(const QByteArray &data);
    // Icoming signals from the client.
    void cancelRequested();
    void writeMessageRequested(const QByteArray &data);
    void writesDoneRequested();

private:
    Q_DISABLE_COPY_MOVE(QGrpcChannelOperation)
    Q_DECLARE_PRIVATE(QGrpcChannelOperation)

    friend class QAbstractGrpcChannel;
public:
    bool event(QEvent *event) override;
};

QT_END_NAMESPACE

#endif // QGRPCCHANNELOPERATION_H
