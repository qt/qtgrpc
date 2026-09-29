// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only

#ifndef QPROTOBUFSERIALIZERBASE_P_H
#define QPROTOBUFSERIALIZERBASE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtProtobuf/qprotobufpropertyordering.h>

#include <QtCore/qtconfigmacros.h>
#include <QtCore/qvarlengtharray.h>

QT_BEGIN_NAMESPACE

namespace QtProtobufPrivate {
struct QProtobufFieldInfo;
}
class QVariant;
class QProtobufMessage;
struct QMetaObject;
class QProtobufSerializerBase
{
public:
    QProtobufSerializerBase();

    void serializeMessage(const QProtobufMessage *message);

    static bool mustSerializeDefaultValue(QtProtobufPrivate::FieldFlags flags)
    {
        return flags.testAnyFlags({ QtProtobufPrivate::FieldFlag::Oneof,
                                    QtProtobufPrivate::FieldFlag::Optional,
                                    QtProtobufPrivate::FieldFlag::Required });
    }

protected:
    ~QProtobufSerializerBase();

    virtual void serializeMessageField(const QProtobufMessage *message,
                                       const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo);

private:
    void serializeDefaultMessageField(const QMetaObject *metaObject,
                                      const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo);

    virtual bool serializeEnum(QVariant &value,
                               const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo) = 0;
    virtual bool serializeScalarField(const QVariant &value,
                                      const QtProtobufPrivate::QProtobufFieldInfo &fieldInfo) = 0;
    virtual void serializeMessageFieldBegin() = 0;
    virtual void serializeMessageFieldEnd(const QProtobufMessage *,
                                          const QtProtobufPrivate::QProtobufFieldInfo &) = 0;

    QVarLengthArray<const QMetaObject *, 4> m_defaultMessageTypes;

    Q_DISABLE_COPY_MOVE(QProtobufSerializerBase)
};

QT_END_NAMESPACE

#endif // QPROTOBUFSERIALIZERBASE_P_H
