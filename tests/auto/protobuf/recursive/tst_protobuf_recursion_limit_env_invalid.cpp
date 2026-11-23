// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtProtobuf/qprotobufserializer.h>

#include <QtCore/qregularexpression.h>
#include <QtCore/qtenvironmentvariables.h>

#include <QtTest/qtest.h>

#include "recursionlimit.qpb.h"

using namespace Qt::StringLiterals;
using namespace qtprotobufnamespace::tests::recursionlimit;

namespace {
// A message with `levels` nested messages in field 2.
QByteArray nested(int levels)
{
    QByteArray data;
    for (int i = 0; i < levels; ++i) {
        QByteArray len;
        quint32 size = quint32(data.size());
        while (size >= 0x80) {
            len.append(char((size & 0x7f) | 0x80));
            size >>= 7;
        }
        len.append(char(size));
        data = QByteArray("\x12") + len + data;
    }
    return data;
}
} // namespace

class tst_protobuf_recursion_limit_env_invalid : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void invalidValueIsIgnored();
};

void tst_protobuf_recursion_limit_env_invalid::invalidValueIsIgnored()
{
    // The variable is read only once per process, so this value is used for
    // the whole test. The warning is printed once, when the value is read.
    qputenv("QT_PROTOBUF_NESTING_LIMIT", "-5");
    auto guard = qScopeGuard([]() {
        qunsetenv("QT_PROTOBUF_NESTING_LIMIT");
    });

    QProtobufSerializer serializer;
    Node node;
    QTest::ignoreMessage(QtWarningMsg, QRegularExpression(u"invalid value"_s));
    // The default limit of 100 is used.
    QVERIFY(serializer.deserialize(&node, nested(100)));
    QVERIFY(!serializer.deserialize(&node, nested(101)));
}

QTEST_MAIN(tst_protobuf_recursion_limit_env_invalid)
#include "tst_protobuf_recursion_limit_env_invalid.moc"
