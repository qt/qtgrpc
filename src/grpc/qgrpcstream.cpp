// Copyright (C) 2022 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtCore/QThread>

#include "qgrpcstream.h"

QT_BEGIN_NAMESPACE

/*!
    \class QGrpcStream
    \inmodule QtGrpc

    \brief The QGrpcStream class.

    TODO: Fill this out with more detail later
*/

/*!
    \fn void messageReceived();

    The signal is emitted when the stream receives an updated value from server.
*/

QGrpcStream::QGrpcStream(const QString &method, const QByteArray &arg, const StreamHandler &handler,
                         QAbstractGrpcClient *parent)
    : QGrpcOperation(parent), m_method(method), m_arg(arg)
{
    if (handler)
        m_handlers.push_back(handler);
}

void QGrpcStream::addHandler(const StreamHandler &handler)
{
    if (handler)
        m_handlers.push_back(handler);
}

/*!
    Cancel this stream and try to abort any call active on any channel
    in the stream.
*/
void QGrpcStream::abort()
{
    if (thread() != QThread::currentThread())
        QMetaObject::invokeMethod(this, &QGrpcStream::finished, Qt::BlockingQueuedConnection);
    else
        finished();
}

/*!
    Returns the method for this stream.
*/
QString QGrpcStream::method() const
{
    return m_method;
}

/*!
    Returns serialized arguments for this stream.
*/
QByteArray QGrpcStream::arg() const
{
    return m_arg;
}

/*!
    Invokes all handler methods assigned to this stream with \a data.

    Should be used by QAbstractGrpcChannel implementations,
    to update data in a stream and notify clients about stream updates.
*/
void QGrpcStream::handler(const QByteArray &data)
{
    setData(data);
    for (auto &handler : m_handlers)
        handler(data);
    emit messageReceived();
}

QT_END_NAMESPACE
