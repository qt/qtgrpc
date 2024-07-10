// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSERIALIZATIONFORMAT_H
#define QGRPCSERIALIZATIONFORMAT_H

#include <QtGrpc/qtgrpcglobal.h>
#include <QtGrpc/qtgrpcnamespace.h>

#include <QtCore/qbytearrayview.h>
#include <QtCore/qtclasshelpermacros.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QGrpcSerializationFormatPrivate;
class QAbstractProtobufSerializer;

class QGrpcSerializationFormat final
{
public:
    Q_GRPC_EXPORT Q_IMPLICIT QGrpcSerializationFormat(QtGrpc::SerializationFormat format = {});
    Q_GRPC_EXPORT QGrpcSerializationFormat(QByteArrayView suffix,
                                           std::shared_ptr<QAbstractProtobufSerializer> serializer);
    Q_GRPC_EXPORT ~QGrpcSerializationFormat();

    Q_GRPC_EXPORT QGrpcSerializationFormat(const QGrpcSerializationFormat &);
    Q_GRPC_EXPORT QGrpcSerializationFormat &operator=(const QGrpcSerializationFormat &);

    QGrpcSerializationFormat(QGrpcSerializationFormat &&other) noexcept = default;
    QT_MOVE_ASSIGNMENT_OPERATOR_IMPL_VIA_MOVE_AND_SWAP(QGrpcSerializationFormat)

    void swap(QGrpcSerializationFormat &other) noexcept { dPtr.swap(other.dPtr); }

    [[nodiscard]] Q_GRPC_EXPORT QByteArray suffix() const noexcept;
    [[nodiscard]] Q_GRPC_EXPORT std::shared_ptr<QAbstractProtobufSerializer>
    serializer() const noexcept;

private:
    std::unique_ptr<QGrpcSerializationFormatPrivate, void (*)(QGrpcSerializationFormatPrivate *)>
        dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCSERIALIZATIONFORMAT_H
