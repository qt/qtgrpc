// Copyright (C) 2025 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtProtobufWellKnownTypes/private/qprotobufwellknowntypesjsonserializers_p.h>
#include <QtProtobufWellKnownTypes/timestamp.qpb.h>

#include <QtProtobuf/private/protobufscalarjsonserializers_p.h>
#include <QtProtobuf/private/qprotobufjsonserializer_p.h>

#include <QtCore/qjsonvalue.h>
#include <QtCore/qtimezone.h>

QT_BEGIN_NAMESPACE

using namespace Qt::StringLiterals;

namespace {
QJsonValue serializeProtobufWellKnownTimestamp(const QProtobufMessage *message)
{
    qint64 secs = 0;
    qint32 nanos = 0;

    if (const auto secondsValue = message->property("seconds"); secondsValue.canConvert<qint64>()) {
        secs = secondsValue.value<qint64>();
    } else {
        qWarning() << "serializeTimestamp() failed to convert seconds";
    }

    if (const auto nanosValue = message->property("nanos"); nanosValue.canConvert<qint32>()) {
        nanos = nanosValue.value<qint32>();
    } else {
        qWarning() << "serializeTimestamp() failed to convert nanos";
    }

    const auto datetime = QDateTime::fromMSecsSinceEpoch(secs * 1000 + nanos / 1000000,
                                                         QTimeZone::UTC);
    return ProtobufScalarJsonSerializers::serializeCommon<
        QString>(datetime.toString(Qt::ISODateWithMs));
}

bool deserializeProtobufWellKnownTimestamp(QProtobufMessage *message, const QJsonValue &value)
{
    if (!value.isString())
        return false;

    const auto tsString = value.toString();
    // Protobuf requires upper-case letters in timestamp string be case sensitive.
    if (tsString.toUpper() != tsString)
        return false;

    if (tsString.contains(u' '))
        return false;

    //Ensure the field either ends with Z or a valid offset
    static const QRegularExpression TimeStampEnding(".+([\\+\\-]\\d{2}:\\d{2}|Z)$"_L1);
    if (!TimeStampEnding.match(tsString).hasMatch())
        return false;

    const auto datetime = QDateTime::fromString(tsString, Qt::ISODateWithMs);

    if (!datetime.isValid()) {
        qWarning() << "deserializeTimestamp() datetime is invalid";
        return false;
    }
    const auto msecs = datetime.toMSecsSinceEpoch();
    const qint64 seconds = msecs / 1000;
    const qint32 nanos = (msecs % 1000) * 1000000;

    message->setProperty("seconds", QVariant::fromValue(seconds));
    message->setProperty("nanos", QVariant::fromValue(nanos));

    return true;
}

} // namespace

void QtProtobufWellKnownTypesPrivate::registerTimestampCustomJsonHandler()
{
    QtProtobufPrivate::registerCustomJsonHandler(QMetaType::fromType<google::protobuf::Timestamp>(),
                                                 serializeProtobufWellKnownTimestamp,
                                                 deserializeProtobufWellKnownTimestamp);
}

QT_END_NAMESPACE
