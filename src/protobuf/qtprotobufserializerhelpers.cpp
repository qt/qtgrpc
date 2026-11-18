// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#include <QtProtobuf/private/qtprotobufserializerhelpers_p.h>
#include <QtProtobuf/private/qprotobufmessage_p.h>
#include <QtProtobuf/private/qtprotobufdefs_p.h>
#include <QtProtobuf/private/qtprotobuflogging_p.h>

#include <QtCore/qtenvironmentvariables.h>

#include <limits>

QT_BEGIN_NAMESPACE

namespace {
constexpr char EnvRecursionLimit[] = "QT_PROTOBUF_NESTING_LIMIT";

// Reading an environment variable takes a global lock, and the limit is needed
// for every nested message. Read it only once per process, the first time it is
// needed (and no explicit limit is set), and cache the result (thread-safe).
const std::optional<int> &envRecursionLimit()
{
    static const std::optional<int> value = [] {
        const auto env = QtProtobufSerializerHelpers::readEnvInteger(
            EnvRecursionLimit, 1, (std::numeric_limits<int>::max)());
        return env ? std::optional<int>(int(*env)) : std::nullopt;
    }();
    return value;
}
} // namespace

QVariant
QtProtobufSerializerHelpers::messageProperty(const QProtobufMessage *message,
                                             const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo,
                                             bool allowInitialize)
{
    const auto *metaObject = QtProtobufSerializerHelpers::messageMetaObject(message);
    int propertyIndex = fieldInfo.propertyIndex() + metaObject->propertyOffset();
    QMetaProperty metaProperty = metaObject->property(propertyIndex);

    if (!metaProperty.isValid())
        return {};

    if (fieldInfo.fieldFlags() & QtProtobufPrivate::FieldFlag::ExplicitPresence
        && !allowInitialize) {
        int hasPropertyIndex = propertyIndex + 1;
        QMetaProperty hasProperty = metaObject->property(hasPropertyIndex);
        Q_ASSERT_X(hasProperty.isValid() && hasProperty.metaType().id() == QMetaType::Bool,
                   "QProtobufMessage",
                   "The field with the explicit presence requirement doesn't have the follow 'has' "
                   "property.");
        if (!hasProperty.readOnGadget(message).toBool())
            return QVariant(metaProperty.metaType());
    }

    QVariant propertyValue = metaProperty.readOnGadget(message);
    return propertyValue;
}

bool QtProtobufSerializerHelpers::setMessageProperty(QProtobufMessage *message,
                                                     const QtProtobufPrivate::QProtobufFieldInfo
                                                         &fieldInfo,
                                                     const QVariant &value)
{
    const auto *pMessage = QProtobufMessagePrivate::get(message);
    const auto mp = pMessage->metaProperty(fieldInfo);
    if (!mp)
        return false;
    return mp->writeOnGadget(message, value);
}

bool QtProtobufSerializerHelpers::setMessageProperty(QProtobufMessage *message,
                                                     const QtProtobufPrivate::QProtobufFieldInfo
                                                         &fieldInfo,
                                                     QVariant &&value)
{
    const auto *pMessage = QProtobufMessagePrivate::get(message);
    const auto mp = pMessage->metaProperty(fieldInfo);
    if (!mp)
        return false;
    return mp->writeOnGadget(message, std::move(value));
}

const QMetaObject *QtProtobufSerializerHelpers::messageMetaObject(const QProtobufMessage *message)
{
    Q_ASSERT(message);
    const auto *pMessage = QProtobufMessagePrivate::get(message);
    Q_ASSERT(pMessage);
    return pMessage->metaObject;
}

bool QtProtobufSerializerHelpers::checkRecursionLimit(int limit, const char *source)
{
    if (limit > 0)
        return true;
    qProtoWarning("Invalid recursion limit (%d) from %s; ignoring.", limit, source);
    return false;
}

std::optional<qint64> QtProtobufSerializerHelpers::readEnvInteger(const char *name, qint64 minimum,
                                                                  qint64 maximum)
{
    if (!qEnvironmentVariableIsSet(name))
        return std::nullopt;
    const auto env = qEnvironmentVariableIntegerValue(name);
    if (!env) {
        qProtoWarning("%s is set to \"%s\", which is not an integer; ignoring.", name,
                      qgetenv(name).constData());
        return std::nullopt;
    }
    if (*env < minimum || *env > maximum) {
        qProtoWarning("%s has an invalid value (%lld); ignoring.", name,
                      static_cast<long long>(*env));
        return std::nullopt;
    }
    return env;
}

int QtProtobufSerializerHelpers::effectiveRecursionLimit(std::optional<int> explicitLimit)
{
    if (explicitLimit)
        return *explicitLimit;
    return envRecursionLimit().value_or(ProtobufDefaultRecursionLimit);
}

QT_END_NAMESPACE
