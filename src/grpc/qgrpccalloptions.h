// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QGRPCALLOPTIONS_H
#define QGRPCALLOPTIONS_H

#include <QtCore/qurl.h>
#include <QtGrpc/qgrpcmetadata.h>
#include <QtGrpc/qtgrpcglobal.h>

#include <chrono>
#include <memory>
#include <optional>

QT_BEGIN_NAMESPACE

struct QGrpcCallOptionsPrivate;

class Q_GRPC_EXPORT QGrpcCallOptions final
{
public:
    QGrpcCallOptions();
    ~QGrpcCallOptions();

    QGrpcCallOptions(const QGrpcCallOptions &other);
    QGrpcCallOptions &operator=(const QGrpcCallOptions &other);
    QGrpcCallOptions(QGrpcCallOptions &&other) noexcept;
    QGrpcCallOptions &operator=(QGrpcCallOptions &&other) noexcept;

    QGrpcCallOptions &withDeadline(std::chrono::milliseconds deadline);
    QGrpcCallOptions &withMetadata(const QGrpcMetadata &metadata);

    [[nodiscard]] std::optional<std::chrono::milliseconds> deadline() const noexcept;
    [[nodiscard]] const QGrpcMetadata &metadata() const noexcept;

private:
    std::unique_ptr<QGrpcCallOptionsPrivate> dPtr;
};

QT_END_NAMESPACE

#endif // QGRPCALLOPTIONS_H
