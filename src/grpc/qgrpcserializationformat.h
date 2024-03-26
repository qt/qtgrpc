// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCSERIALIZATIONFORMAT_H
#define QGRPCSERIALIZATIONFORMAT_H

#include <QtCore/qbytearrayview.h>
#include <QtCore/qtclasshelpermacros.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <memory>

QT_BEGIN_NAMESPACE

class QGrpcSerializationFormatPrivate;
class QAbstractProtobufSerializer;

class Q_GRPC_EXPORT QGrpcSerializationFormat final
{
public:
    enum Format : quint8 {
        Default = 0,
        Protobuf,
        Json,
    };

    QGrpcSerializationFormat(Format format = Format::Default);
    QGrpcSerializationFormat(QByteArrayView suffix,
                             std::shared_ptr<QAbstractProtobufSerializer> serializer);

    ~QGrpcSerializationFormat();

    QGrpcSerializationFormat(const QGrpcSerializationFormat &);
    QGrpcSerializationFormat(QGrpcSerializationFormat &&);

    QGrpcSerializationFormat &operator=(const QGrpcSerializationFormat &);
    QGrpcSerializationFormat &operator=(QGrpcSerializationFormat &&);

    [[nodiscard]] QByteArray suffix() const noexcept;
    [[nodiscard]] std::shared_ptr<QAbstractProtobufSerializer> serializer() const noexcept;

private:
    std::unique_ptr<QGrpcSerializationFormatPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCSERIALIZATIONFORMAT_H
