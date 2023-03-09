// Copyright (C) 2023 The Qt Company Ltd.
// Copyright (C) 2019 Alexey Edelev <semlanik@gmail.com>
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

#include "qtprotobufqtcoretypes.h"
#include "qtprotobufqttypescommon_p.h"
#include "private/QtCore.qpb.h"

#include <QtCore/qurl.h>
#include <QtCore/qchar.h>
#include <QtCore/quuid.h>
#include <QtCore/qtimezone.h>
#include <QtCore/qdatetime.h>
#include <QtCore/qdatastream.h>
#include <QtCore/qsize.h>
#include <QtCore/qpoint.h>
#include <QtCore/qrect.h>
#include <QtCore/qbuffer.h>
#include <QtCore/qversionnumber.h>
#include <QtCore/qendian.h>

QT_BEGIN_NAMESPACE

static std::optional<QUrl> convert(const QtProtobufPrivate::QtCore::QUrl &from)
{
    QUrl url(from.url());
    return (url.isValid() || url.isEmpty()) ? std::optional<QUrl>(url) : std::nullopt;
}

static std::optional<QtProtobufPrivate::QtCore::QUrl> convert(const QUrl &from)
{
    if (from.isValid() || from.isEmpty()) {
        QtProtobufPrivate::QtCore::QUrl url;
        url.setUrl(from.url());
        return url;
    }
    return std::nullopt;
}

static QChar convert(const QtProtobufPrivate::QtCore::QChar &from)
{
    return QChar(from.utf16CodePoint());
}

static QtProtobufPrivate::QtCore::QChar convert(const QChar &from)
{
    QtProtobufPrivate::QtCore::QChar symbol;
    symbol.setUtf16CodePoint(from.unicode());
    return symbol;
}

static std::optional<QUuid> convert(const QtProtobufPrivate::QtCore::QUuid &from)
{
    if (from.rfc4122Uuid().size() != 16)
        return std::nullopt;

    return QUuid::fromRfc4122(from.rfc4122Uuid());
}

static std::optional<QtProtobufPrivate::QtCore::QUuid> convert(const QUuid &from)
{
    if (from.toRfc4122().size() != 16)
       return std::nullopt;

    QtProtobufPrivate::QtCore::QUuid uuid;
    uuid.setRfc4122Uuid(from.toRfc4122());
    return uuid;
}

static std::optional<QTime> convert(const QtProtobufPrivate::QtCore::QTime &from)
{
    QTime time = QTime::fromMSecsSinceStartOfDay(from.millisecondsSinceMidnight());
    return time.isValid() ? std::optional<QTime>(time) : std::nullopt;
}

static std::optional<QtProtobufPrivate::QtCore::QTime> convert(const QTime &from)
{
    if (!from.isValid())
        return std::nullopt;

    QtProtobufPrivate::QtCore::QTime time;
    time.setMillisecondsSinceMidnight(from.msecsSinceStartOfDay());
    return time;
}

static std::optional<QDate> convert(const QtProtobufPrivate::QtCore::QDate &from)
{
    QDate date = QDate::fromJulianDay(from.julianDay());
    return date.isValid() ? std::optional<QDate>(date) : std::nullopt;
}

static std::optional<QtProtobufPrivate::QtCore::QDate> convert(const QDate &from)
{
    if (!from.isValid())
        return std::nullopt;

    QtProtobufPrivate::QtCore::QDate date;
    date.setJulianDay(from.toJulianDay());
    return date;
}

static std::optional<QDateTime> convert(const QtProtobufPrivate::QtCore::QDateTime &from)
{
    QDateTime dateTime(QDateTime::fromMSecsSinceEpoch(from.utcMsecsSinceUnixEpoch(),
                                                      QTimeZone::UTC));
    return dateTime.isValid() ? std::optional<QDateTime>(dateTime) : std::nullopt;
}

static std::optional<QtProtobufPrivate::QtCore::QDateTime> convert(const QDateTime &from)
{
    if (!from.isValid())
        return std::nullopt;

    QtProtobufPrivate::QtCore::QDateTime datetime;
    datetime.setUtcMsecsSinceUnixEpoch(from.toMSecsSinceEpoch());
    return datetime;
}

static QSize convert(const QtProtobufPrivate::QtCore::QSize &from)
{
    return QSize(from.width(), from.height());
}

static QtProtobufPrivate::QtCore::QSize convert(const QSize &from)
{
    QtProtobufPrivate::QtCore::QSize size;
    size.setWidth(from.width());
    size.setHeight(from.height());
    return size;
}

static QSizeF convert(const QtProtobufPrivate::QtCore::QSizeF &from)
{
    return QSizeF(from.width(), from.height());
}

static QtProtobufPrivate::QtCore::QSizeF convert(const QSizeF &from)
{
    QtProtobufPrivate::QtCore::QSizeF sizeF;
    sizeF.setWidth(from.width());
    sizeF.setHeight(from.height());
    return sizeF;
}

static QPoint convert(const QtProtobufPrivate::QtCore::QPoint &from)
{
    return QPoint(from.x(), from.y());
}

static QtProtobufPrivate::QtCore::QPoint convert(const QPoint &from)
{
    QtProtobufPrivate::QtCore::QPoint pointT;
    pointT.setX(from.x());
    pointT.setY(from.y());
    return pointT;
}

static QPointF convert(const QtProtobufPrivate::QtCore::QPointF &from)
{
    return QPointF(from.x(), from.y());
}

static QtProtobufPrivate::QtCore::QPointF convert(const QPointF &from)
{
    QtProtobufPrivate::QtCore::QPointF pointF;
    pointF.setX(from.x());
    pointF.setY(from.y());
    return pointF;
}

static QRect convert(const QtProtobufPrivate::QtCore::QRect &from)
{
    return QRect(QPoint(from.x(), from.y()),
                 QSize(from.width(), from.height()));
}

static QtProtobufPrivate::QtCore::QRect convert(const QRect &from)
{
    QtProtobufPrivate::QtCore::QRect rect;
    rect.setX(from.x());
    rect.setY(from.y());
    rect.setWidth(from.width());
    rect.setHeight(from.height());
    return rect;
}

static QRectF convert(const QtProtobufPrivate::QtCore::QRectF &from)
{
    return QRectF(QPointF(from.x(), from.y()),
                  QSizeF(from.width(), from.height()));
}

static QtProtobufPrivate::QtCore::QRectF convert(const QRectF &from)
{
    QtProtobufPrivate::QtCore::QRectF rectF;
    rectF.setX(from.x());
    rectF.setY(from.y());
    rectF.setWidth(from.width());
    rectF.setHeight(from.height());
    return rectF;
}

static std::optional<QVersionNumber> convert(const QtProtobufPrivate::QtCore::QVersionNumber &from)
{
    if (from.segments().size() == 0)
        return std::nullopt;

    QList<int> versionList;
    const auto segments = from.segments();
    for (const auto &segment : segments)
        versionList.append(segment);
    return QVersionNumber(versionList);
}

static std::optional<QtProtobufPrivate::QtCore::QVersionNumber> convert(const QVersionNumber &from)
{
    if (from.segments().size() == 0)
        return std::nullopt;

    QtProtobufPrivate::QtCore::QVersionNumber version;
    const auto segments = from.segments();
    for (const auto &segment : segments)
        version.segments().append(segment);
    return version;
}

namespace QtProtobuf {

/*!
    Registers serializers for the Qt::ProtobufQtCoreTypes library.
*/
void qRegisterProtobufQtCoreTypes() {
    QtProtobufPrivate::registerQtTypeHandler<QUrl, QtProtobufPrivate::QtCore::QUrl>();
    QtProtobufPrivate::registerQtTypeHandler<QChar, QtProtobufPrivate::QtCore::QChar>();
    QtProtobufPrivate::registerQtTypeHandler<QUuid, QtProtobufPrivate::QtCore::QUuid>();
    QtProtobufPrivate::registerQtTypeHandler<QTime, QtProtobufPrivate::QtCore::QTime>();
    QtProtobufPrivate::registerQtTypeHandler<QDate, QtProtobufPrivate::QtCore::QDate>();
    QtProtobufPrivate::registerQtTypeHandler<QDateTime, QtProtobufPrivate::QtCore::QDateTime>();
    QtProtobufPrivate::registerQtTypeHandler<QSize, QtProtobufPrivate::QtCore::QSize>();
    QtProtobufPrivate::registerQtTypeHandler<QSizeF, QtProtobufPrivate::QtCore::QSizeF>();
    QtProtobufPrivate::registerQtTypeHandler<QPoint, QtProtobufPrivate::QtCore::QPoint>();
    QtProtobufPrivate::registerQtTypeHandler<QPointF, QtProtobufPrivate::QtCore::QPointF>();
    QtProtobufPrivate::registerQtTypeHandler<QRect, QtProtobufPrivate::QtCore::QRect>();
    QtProtobufPrivate::registerQtTypeHandler<QRectF, QtProtobufPrivate::QtCore::QRectF>();
    QtProtobufPrivate::registerQtTypeHandler<QVersionNumber,
            QtProtobufPrivate::QtCore::QVersionNumber>();
}
}

QT_END_NAMESPACE
