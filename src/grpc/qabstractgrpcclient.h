// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTGRPCLIENT_H
#define QABSTRACTGRPCLIENT_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QWeakPointer>
#include <QtCore/qbytearray.h>
#include <QtGrpc/qabstractgrpcchannel.h>
#include <QtGrpc/qgrpcstatus.h>
#include <QtGrpc/qtgrpcglobal.h>
#include <QtProtobuf/qabstractprotobufserializer.h>

#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

class QGrpcOperation;
class QAbstractGrpcChannel;
class QAbstractGrpcClientPrivate;

class Q_GRPC_EXPORT QAbstractGrpcClient : public QObject
{
    Q_OBJECT
    using StreamHandler = std::function<void(const QByteArray &)>;

public:
    void attachChannel(const std::shared_ptr<QAbstractGrpcChannel> &channel);

Q_SIGNALS:
    void errorOccurred(const QGrpcStatus &status) const;

protected:
    explicit QAbstractGrpcClient(const QString &service, QObject *parent = nullptr);
    ~QAbstractGrpcClient() override;

    template <typename ParamType, typename ReturnType>
    QGrpcStatus call(const QString &method, const QProtobufMessage &arg, ReturnType *ret)
    {
        if (ret == nullptr) {
            const auto errorString = QLatin1StringView("Unable to call method: %1. "
                                                       "Pointer to return data is null.")
                                             .arg(method);
            QGrpcStatus status{ QGrpcStatus::InvalidArgument, errorString };
            errorOccurred(status);
            logError(errorString);
            return status;
        }
        QGrpcStatus status{ QGrpcStatus::Unknown,
                            QLatin1StringView("Serializing failed. "
                                              "Serializer is not ready.") };

        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
        if (argData) {
            QByteArray retData;
            status = call(method, *argData, retData);
            if (status == QGrpcStatus::StatusCode::Ok)
                status = tryDeserialize(ret, retData);
        }
        return status;
    }

    template <typename ParamType>
    std::shared_ptr<QGrpcCallReply> call(const QString &method, const QProtobufMessage &arg)
    {
        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
        if (!argData)
            return {};
        return call(method, *argData);
    }

    template <typename ParamType>
    std::shared_ptr<QGrpcStream> stream(const QString &method, const QProtobufMessage &arg)
    {
        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
        if (!argData)
            return {};
        return stream(method, *argData);
    }

    template <typename ParamType, typename ReturnType>
    std::shared_ptr<QGrpcStream> stream(const QString &method, const QProtobufMessage &arg,
                                        const QWeakPointer<ReturnType> ret)
    {
        if (ret.isNull()) {
            const auto nullPointerError = QLatin1StringView("Unable to stream method: %1. "
                                                            "Pointer to return data is null.")
                                                  .arg(method);
            errorOccurred({ QGrpcStatus::InvalidArgument, nullPointerError });
            logError(nullPointerError);
            return {};
        }
        std::optional<QByteArray> argData = trySerialize<ParamType>(arg);
        if (!argData)
            return {};

        return stream(method, *argData, [ret, this](const QByteArray &data) {
            if (auto retVal = ret.lock()) {
                auto status = tryDeserialize(retVal.get(), data);
                if (status != QGrpcStatus::Ok) {
                    errorOccurred(status);
                    logError(status.message());
                }
            } else {
                static const QLatin1StringView nullPointerError("Pointer to return data is null "
                                                                "while stream update received");
                errorOccurred({ QGrpcStatus::InvalidArgument, nullPointerError });
                logError(nullPointerError);
            }
        });
    }

    friend class QGrpcOperation;

private:
    QGrpcStatus call(const QString &method, const QByteArray &arg, QByteArray &ret);

    std::shared_ptr<QGrpcCallReply> call(const QString &method, const QByteArray &arg);

    std::shared_ptr<QGrpcStream> stream(const QString &method, const QByteArray &arg,
                                        const StreamHandler &handler = {});

    template <typename ReturnType>
    QGrpcStatus tryDeserialize(ReturnType *ret, QByteArrayView retData)
    {
        auto _serializer = serializer();
        if (_serializer == nullptr)
            return QGrpcStatus::Unknown;
        if (!_serializer->deserialize(ret, retData))
            return handleDeserializationError(_serializer->deserializationError());
        return QGrpcStatus::Ok;
    }

    template <typename ParamType>
    std::optional<QByteArray> trySerialize(const QProtobufMessage &arg)
    {
        auto _serializer = serializer();
        if (_serializer == nullptr) {
            errorOccurred({ QGrpcStatus::Unknown,
                            QLatin1StringView("Serializing failed. Serializer is not ready.") });
            return std::nullopt;
        }
        return _serializer->serialize<ParamType>(&arg);
    }

    std::shared_ptr<QAbstractProtobufSerializer> serializer() const;

    void logError(const QString &str) const;
    QGrpcStatus handleDeserializationError(
            const QAbstractProtobufSerializer::DeserializationError &err) const;

    Q_DISABLE_COPY_MOVE(QAbstractGrpcClient)
    Q_DECLARE_PRIVATE(QAbstractGrpcClient)
};

QT_END_NAMESPACE

#endif // QABSTRACTGRPCLIENT_H
