// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCOPERATION_H
#define QGRPCOPERATION_H

#include <QtCore/qobject.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtGrpc/qgrpcdefs.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

QT_BEGIN_NAMESPACE

class QGrpcChannelOperation;
class QGrpcOperationPrivate;

class Q_GRPC_EXPORT QGrpcOperation : public QObject
{
    Q_OBJECT

public:
    ~QGrpcOperation() override;

    template <typename T>
    std::optional<T> read() const
    {
        T value;
        return read(&value) ? std::optional<T>(value) : std::nullopt;
    }

    bool read(QProtobufMessage *message) const;

    [[nodiscard]] QAbstractProtobufSerializer::DeserializationError deserializationError() const;
    [[nodiscard]] QString deserializationErrorString() const;

    [[nodiscard]] const QGrpcMetadata &metadata() const noexcept;
    [[nodiscard]] QLatin1StringView method() const noexcept;

    void cancel();
    [[nodiscard]] bool isFinished() const noexcept;

Q_SIGNALS:
    void finished();
    void errorOccurred(const QGrpcStatus &status);

protected:
    explicit QGrpcOperation(std::shared_ptr<QGrpcChannelOperation> channelOperation,
                            QObject *parent = nullptr);

    [[nodiscard]] QGrpcChannelOperation *channelOperation() const noexcept;

private:
    Q_DISABLE_COPY_MOVE(QGrpcOperation)
    Q_DECLARE_PRIVATE(QGrpcOperation)

    [[nodiscard]] QByteArray data() const noexcept;
};

QT_END_NAMESPACE

#endif // QGRPCOPERATION_H
