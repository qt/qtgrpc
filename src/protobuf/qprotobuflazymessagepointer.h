// Copyright (C) 2020 Alexey Edelev <semlanik@gmail.com>
// Copyright (C) 2022 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFLAZYMESSAGEPOINTER_H
#define QPROTOBUFLAZYMESSAGEPOINTER_H

#include <QtProtobuf/qtprotobufglobal.h>

#include <QtCore/qobject.h>
#include <QtCore/qpointer.h>

#include <type_traits>

QT_BEGIN_NAMESPACE
namespace QtProtobufPrivate {
class QProtobufLazyMessagePointerBase
{
    Q_DISABLE_COPY(QProtobufLazyMessagePointerBase)
    mutable QPointer<QObject> m_ptr;

protected:
    QProtobufLazyMessagePointerBase() = default;
    explicit QProtobufLazyMessagePointerBase(QObject *ptr) : m_ptr(ptr) { }
    QProtobufLazyMessagePointerBase(QProtobufLazyMessagePointerBase &&other) noexcept = default;
    QProtobufLazyMessagePointerBase &operator=(QProtobufLazyMessagePointerBase &&other) noexcept = default;

    QObject *object() const { return m_ptr.get(); }
    void setObject(QObject *obj) const { m_ptr = obj; }

    ~QProtobufLazyMessagePointerBase()
    {
        delete m_ptr.get();
    }

    void swap(QProtobufLazyMessagePointerBase &other) noexcept { m_ptr.swap(other.m_ptr); }
    explicit operator bool() const noexcept { return object() != nullptr; }
};

template <typename T>
class QProtobufLazyMessagePointer : private QProtobufLazyMessagePointerBase
{
    T *objectAsT() const { return static_cast<T *>(object()); }

public:
    QProtobufLazyMessagePointer() noexcept = default;
    explicit QProtobufLazyMessagePointer(T *p) { setObject(p); }
    QProtobufLazyMessagePointer(QProtobufLazyMessagePointer &&other) noexcept = default;
    QProtobufLazyMessagePointer &operator=(QProtobufLazyMessagePointer &&other) noexcept = default;
    ~QProtobufLazyMessagePointer() = default; // We don't own anything + base is not virtual

    T &operator*() const { return *get(); }
    T *operator->() const { return get(); }
    T *get() const
    {
        if (!*this) setObject(new T);
        return objectAsT();
    }

    void reset(T *p = nullptr)
    {
        Q_ASSERT(!p || object() != p);
        QProtobufLazyMessagePointer(p).swap(*this);
    }

    void swap(QProtobufLazyMessagePointer &other) noexcept
    {
        QProtobufLazyMessagePointerBase::swap(other);
    }

    using QProtobufLazyMessagePointerBase::operator bool;

private:
    friend bool operator==(const QProtobufLazyMessagePointer &lhs,
                           const QProtobufLazyMessagePointer &rhs) noexcept
    {
        T *lhsPtr = lhs.objectAsT();
        T *rhsPtr = rhs.objectAsT();
        return (lhsPtr == nullptr && rhsPtr == nullptr)
                || (lhsPtr != nullptr && rhsPtr != nullptr && *lhsPtr == *rhsPtr);
    }
    friend bool operator!=(const QProtobufLazyMessagePointer &lhs,
                           const QProtobufLazyMessagePointer &rhs) noexcept
    {
        return !operator==(lhs, rhs);
    }
};
} // namespace QtProtobufPrivate

QT_END_NAMESPACE
#endif // QPROTOBUFLAZYMESSAGEPOINTER_H
