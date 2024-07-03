// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGrpc/qgrpcdefs.h>

#include <QtCore/qdebug.h>

/*!
    \typealias QGrpcMetadata
    \inmodule QtGrpc
    \relates std::unordered_multimap<QByteArray, QByteArray>
    \brief The QGrpcMetadata is an STL-style
    \l {https://en.cppreference.com/w/cpp/container/unordered_multimap} {unordered
    multimap container}.

    Metadata contains information about a particular RPC call or channel
    (such as authentication details) in the form of a list of key-value pairs,
    where the keys are strings and the values are typically strings, but can
    be binary data.
*/

QT_BEGIN_NAMESPACE

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug debug, const QGrpcMetadata &metadata)
{
    QDebugStateSaver save(debug);
    debug.nospace();
    debug.noquote();
    debug << "QGrpcMetadata(";
    size_t i = 0;
    for (const auto &md : metadata) {
        debug << "{key: " << md.first << ", value: " << md.second;
        if (++i < metadata.size())
            debug << "}, ";
        else
            debug << '}';
    }
    debug << ')';
    return debug;
}
#endif

QT_END_NAMESPACE
