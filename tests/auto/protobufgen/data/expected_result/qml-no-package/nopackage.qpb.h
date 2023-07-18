/* This file is autogenerated. DO NOT CHANGE. All changes will be lost */

#ifndef QPROTOBUF_NOPACKAGE_H
#define QPROTOBUF_NOPACKAGE_H

#include <QtProtobuf/qprotobufmessage.h>
#include <QtProtobuf/qprotobufobject.h>
#include <QtProtobuf/qprotobuflazymessagepointer.h>

#include <QtQml/qqmlregistration.h>
#include <QtQml/qqmllist.h>

#include <QtCore/qbytearray.h>
#include <QtCore/qstring.h>
#include "nopackageexternal.qpb.h"

#include <QtCore/qmetatype.h>
#include <QtCore/qlist.h>
#include <QtCore/qshareddata.h>

#include <memory>

#if defined(QT_SHARED) || !defined(QT_STATIC)
#  if defined(QT_BUILD_TST_QTPROTOBUFGENPLUGIN_LIB)
#    define QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT Q_DECL_EXPORT
#  else
#    define QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT
#endif


namespace TestEnumGadget {
Q_NAMESPACE_EXPORT(QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT)
QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT void registerTypes();
enum TestEnum {
    LOCAL_ENUM_VALUE0 = 0,
    LOCAL_ENUM_VALUE1 = 1,
    LOCAL_ENUM_VALUE2 = 2,
    LOCAL_ENUM_VALUE3 = 5,
};
Q_ENUM_NS(TestEnum)
using TestEnumRepeated = QList<TestEnum>;
};
class EmptyMessage;
using EmptyMessageRepeated = QList<EmptyMessage>;
class SimpleIntMessage;
using SimpleIntMessageRepeated = QList<SimpleIntMessage>;
class NoPackageExternalMessage;
using NoPackageExternalMessageRepeated = QList<NoPackageExternalMessage>;
class NoPackageMessage;
using NoPackageMessageRepeated = QList<NoPackageMessage>;

class EmptyMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT EmptyMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(EmptyMessage)
    QML_VALUE_TYPE(emptyMessage)

public:
    EmptyMessage();
    ~EmptyMessage();
    EmptyMessage(const EmptyMessage &other);
    EmptyMessage &operator =(const EmptyMessage &other);
    EmptyMessage(EmptyMessage &&other) noexcept;
    EmptyMessage &operator =(EmptyMessage &&other) noexcept;
    bool operator ==(const EmptyMessage &other) const;
    bool operator !=(const EmptyMessage &other) const;
    static void registerTypes();

private:
    QExplicitlySharedDataPointer<EmptyMessage_QtProtobufData> dptr;
};

class SimpleIntMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT SimpleIntMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(SimpleIntMessage)
    QML_VALUE_TYPE(simpleIntMessage)
    Q_PROPERTY(QtProtobuf::int32 testFieldInt_p READ testFieldInt WRITE setTestFieldInt SCRIPTABLE false)
    Q_PROPERTY(int testFieldInt READ testFieldInt_p WRITE setTestFieldInt_p SCRIPTABLE true)

public:
    enum QtProtobufFieldEnum {
        TestFieldIntProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    SimpleIntMessage();
    ~SimpleIntMessage();
    SimpleIntMessage(const SimpleIntMessage &other);
    SimpleIntMessage &operator =(const SimpleIntMessage &other);
    SimpleIntMessage(SimpleIntMessage &&other) noexcept;
    SimpleIntMessage &operator =(SimpleIntMessage &&other) noexcept;
    bool operator ==(const SimpleIntMessage &other) const;
    bool operator !=(const SimpleIntMessage &other) const;

    QtProtobuf::int32 testFieldInt() const;
    void setTestFieldInt(const QtProtobuf::int32 &testFieldInt);
    static void registerTypes();

private:
    int testFieldInt_p() const;
    void setTestFieldInt_p(const int &testFieldInt);
    QExplicitlySharedDataPointer<SimpleIntMessage_QtProtobufData> dptr;
};

class NoPackageExternalMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT NoPackageExternalMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NoPackageExternalMessage)
    QML_VALUE_TYPE(noPackageExternalMessage)
    Q_PROPERTY(SimpleIntMessageExt *testField_p READ testField_p WRITE setTestField_p)
    Q_PROPERTY(SimpleIntMessageExt testField READ testField WRITE setTestField)

public:
    enum QtProtobufFieldEnum {
        TestFieldProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    NoPackageExternalMessage();
    ~NoPackageExternalMessage();
    NoPackageExternalMessage(const NoPackageExternalMessage &other);
    NoPackageExternalMessage &operator =(const NoPackageExternalMessage &other);
    NoPackageExternalMessage(NoPackageExternalMessage &&other) noexcept;
    NoPackageExternalMessage &operator =(NoPackageExternalMessage &&other) noexcept;
    bool operator ==(const NoPackageExternalMessage &other) const;
    bool operator !=(const NoPackageExternalMessage &other) const;

    SimpleIntMessageExt &testField() const;
    void clearTestField();
    void setTestField(const SimpleIntMessageExt &testField);
    static void registerTypes();

private:
    SimpleIntMessageExt *testField_p() const;
    void setTestField_p(SimpleIntMessageExt *testField);
    QExplicitlySharedDataPointer<NoPackageExternalMessage_QtProtobufData> dptr;
};

class NoPackageMessage_QtProtobufData;
class QPB_TST_QTPROTOBUFGENPLUGIN_EXPORT NoPackageMessage : public QProtobufMessage
{
    Q_GADGET
    Q_PROTOBUF_OBJECT
    Q_DECLARE_PROTOBUF_SERIALIZERS(NoPackageMessage)
    QML_VALUE_TYPE(noPackageMessage)
    Q_PROPERTY(SimpleIntMessage *testField_p READ testField_p WRITE setTestField_p)
    Q_PROPERTY(SimpleIntMessage testField READ testField WRITE setTestField)

public:
    enum QtProtobufFieldEnum {
        TestFieldProtoFieldNumber = 1,
    };
    Q_ENUM(QtProtobufFieldEnum)

    NoPackageMessage();
    ~NoPackageMessage();
    NoPackageMessage(const NoPackageMessage &other);
    NoPackageMessage &operator =(const NoPackageMessage &other);
    NoPackageMessage(NoPackageMessage &&other) noexcept;
    NoPackageMessage &operator =(NoPackageMessage &&other) noexcept;
    bool operator ==(const NoPackageMessage &other) const;
    bool operator !=(const NoPackageMessage &other) const;

    SimpleIntMessage &testField() const;
    void clearTestField();
    void setTestField(const SimpleIntMessage &testField);
    static void registerTypes();

private:
    SimpleIntMessage *testField_p() const;
    void setTestField_p(SimpleIntMessage *testField);
    QExplicitlySharedDataPointer<NoPackageMessage_QtProtobufData> dptr;
};

Q_DECLARE_METATYPE(EmptyMessage)
Q_DECLARE_METATYPE(SimpleIntMessage)
Q_DECLARE_METATYPE(NoPackageExternalMessage)
Q_DECLARE_METATYPE(NoPackageMessage)
#endif // QPROTOBUF_NOPACKAGE_H
