// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFREPEATEDITERATOR_H
#define QPROTOBUFREPEATEDITERATOR_H

#include <QtProtobuf/qtprotobufexports.h>

#include <QtCore/qhash.h>
#include <QtCore/qlist.h>

QT_BEGIN_NAMESPACE

class QProtobufMessage;

namespace QtProtobufPrivate {
class AbstractRepeatedIterator;
}

class QProtobufRepeatedIterator
{
public:
    QProtobufRepeatedIterator() noexcept = default;
    Q_PROTOBUF_EXPORT explicit QProtobufRepeatedIterator(QtProtobufPrivate::AbstractRepeatedIterator
                                                             *data) noexcept;
    Q_PROTOBUF_EXPORT ~QProtobufRepeatedIterator();
    QProtobufRepeatedIterator(QProtobufRepeatedIterator &&other) noexcept
        : m_data(std::exchange(other.m_data, nullptr))
    {
    }
    QProtobufRepeatedIterator &operator=(QProtobufRepeatedIterator &&other) noexcept
    {
        qt_ptr_swap(m_data, other.m_data);
        return *this;
    };

    [[nodiscard]] bool isValid() const noexcept { return m_data != nullptr; }

    [[nodiscard]] Q_PROTOBUF_EXPORT bool hasNext() const noexcept;
    [[nodiscard]] Q_PROTOBUF_EXPORT QProtobufMessage &next() const;

    [[nodiscard]] Q_PROTOBUF_EXPORT QProtobufMessage &addNext();
    Q_PROTOBUF_EXPORT void push() const;

private:
    Q_DISABLE_COPY(QProtobufRepeatedIterator)

    QtProtobufPrivate::AbstractRepeatedIterator *m_data = nullptr;
};

QT_END_NAMESPACE

#endif // QPROTOBUFREPEATEDITERATOR_H
