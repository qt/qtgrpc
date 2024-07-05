// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCOPERATION_H
#define QGRPCOPERATION_H

#include <QtGrpc/qgrpcdefs.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <QtProtobuf/qabstractprotobufserializer.h>

#include <QtCore/qobject.h>

#include <optional>

QT_BEGIN_NAMESPACE

class QGrpcOperationContext;
class QGrpcOperationPrivate;

class Q_GRPC_EXPORT QGrpcOperation : public QObject
{
    Q_OBJECT

public:
    ~QGrpcOperation() override;

    template <typename T>
    std::optional<T> read() const
    {
        std::optional<T> r(std::in_place);
        if (!read(&r.value()))
            r.reset();
        return r;
    }
    bool read(QProtobufMessage *message) const;

    [[nodiscard]] QAbstractProtobufSerializer::DeserializationError deserializationError() const;
    [[nodiscard]] QString deserializationErrorString() const;

    [[nodiscard]] const QGrpcMetadata &metadata() const & noexcept;
    void metadata() const && = delete;

    [[nodiscard]] QLatin1StringView method() const noexcept;

    [[nodiscard]] bool isFinished() const noexcept;

Q_SIGNALS:
    void finished(const QGrpcStatus &status);

public Q_SLOTS:
    void cancel();

protected:
    explicit QGrpcOperation(std::shared_ptr<QGrpcOperationContext> operationContext,
                            QObject *parent = nullptr);

    [[nodiscard]] QGrpcOperationContext *operationContext() const noexcept;

private:
    Q_DISABLE_COPY_MOVE(QGrpcOperation)
    Q_DECLARE_PRIVATE(QGrpcOperation)
};

QT_END_NAMESPACE

#endif // QGRPCOPERATION_H
