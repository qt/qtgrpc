/****************************************************************************
**
** Copyright (C) 2023 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick
import QtTest

import qtprotobufnamespace.tests

TestCase {
    name: "QtProtobuf basic messages test"
    property simpleBoolMessage boolMsg;
    property simpleIntMessage int32Msg;
    property simpleSIntMessage sint32Msg;
    property simpleUIntMessage uint32Msg;
    property simpleFixedInt32Message fixed32Msg;
    property simpleSFixedInt32Message sfixed32Msg;
    property simpleStringMessage stringMsg;
    property complexMessage complexMsg;
    property simpleStringMessage outerMessage;
    property simpleStringMessage innerMessage;

    function test_1initialization() {
        int32Msg.testFieldInt = 2147483647
        sint32Msg.testFieldInt = 2147483647
        uint32Msg.testFieldInt = 4294967295
        fixed32Msg.testFieldFixedInt32 = 4294967295
        sfixed32Msg.testFieldFixedInt32 = 2147483647
        stringMsg.testFieldString = "Test string"
        outerMessage.testFieldString = "outer"
        innerMessage.testFieldString = "inner"
        complexMsg.testComplexField = innerMessage
        complexMsg.testComplexField.testFieldString = "inner"
    }

    function test_1initializationCheck_data() {
        return [
                    {tag: "SimpleIntMessage initialization",
                        field: int32Msg.testFieldInt, answer: 2147483647 },
                    {tag: "SimpleSIntMessage initialization",
                        field: sint32Msg.testFieldInt, answer: 2147483647 },
                    {tag: "SimpleUIntMessage initialization",
                        field: uint32Msg.testFieldInt, answer: 4294967295 },
                    {tag: "SimpleFixedInt32Message initialization",
                        field: fixed32Msg.testFieldFixedInt32, answer: 4294967295 },
                    {tag: "SimpleSFixedInt32Message initialization",
                        field: sfixed32Msg.testFieldFixedInt32, answer: 2147483647 },
                ]
    }

    function test_1initializationCheck(data) {
        compare(data.field, data.answer)
    }

    function test_simpleboolmessage_data() {
        return [
                    {tag: "SimpleBoolMessage == true",
                        field: boolMsg.testFieldBool = true, answer: true },
                    {tag: "SimpleBoolMessage == false",
                        field: boolMsg.testFieldBool = false, answer: false },
                ]
    }

    function test_simpleboolmessage(data) {
        compare(data.field, data.answer)
    }

    function test_simpleintmessage_data() {
        return [
                    {tag: "SimpleIntMessage == 0",
                        field: int32Msg.testFieldInt = 0, answer: 0 },
                    {tag: "SimpleIntMessage == -128",
                        field: int32Msg.testFieldInt = -128, answer: -128 },
                    {tag: "SimpleIntMessage == 127",
                        field: int32Msg.testFieldInt = 127, answer: 127 },
                    {tag: "SimpleIntMessage == -256",
                        field: int32Msg.testFieldInt = -256, answer: -256 },
                    {tag: "SimpleIntMessage == 255",
                        field: int32Msg.testFieldInt = 255, answer: 255 },
                    {tag: "SimpleIntMessage == -32768",
                        field: int32Msg.testFieldInt = -32768, answer: -32768 },
                    {tag: "SimpleIntMessage == 32767",
                        field: int32Msg.testFieldInt = 32767, answer: 32767 },
                    {tag: "SimpleIntMessage == -65536",
                        field: int32Msg.testFieldInt = -65536, answer: -65536 },
                    {tag: "SimpleIntMessage == 65535",
                        field: int32Msg.testFieldInt = 65535, answer: 65535 },
                    {tag: "SimpleIntMessage == -2147483648",
                        field: int32Msg.testFieldInt = -2147483648, answer: -2147483648 },
                    {tag: "SimpleIntMessage == 2147483647",
                        field: int32Msg.testFieldInt = 2147483647, answer: 2147483647 },
                ]
    }

    function test_simpleintmessage(data) {
        compare(data.field, data.answer)
    }

    function test_simplesintmessage_data() {
        return [
                    {tag: "SimpleSIntMessage == 0",
                        field: sint32Msg.testFieldInt = 0, answer: 0 },
                    {tag: "SimpleSIntMessage == -128",
                        field: sint32Msg.testFieldInt = -128, answer: -128 },
                    {tag: "SimpleSIntMessage == 127",
                        field: sint32Msg.testFieldInt = 127, answer: 127 },
                    {tag: "SimpleSIntMessage == -256",
                        field: sint32Msg.testFieldInt = -256, answer: -256 },
                    {tag: "SimpleSIntMessage == 255",
                        field: sint32Msg.testFieldInt = 255, answer: 255 },
                    {tag: "SimpleSIntMessage == -32768",
                        field: sint32Msg.testFieldInt = -32768, answer: -32768 },
                    {tag: "SimpleSIntMessage == 32767",
                        field: sint32Msg.testFieldInt = 32767, answer: 32767 },
                    {tag: "SimpleSIntMessage == -65536",
                        field: sint32Msg.testFieldInt = -65536, answer: -65536 },
                    {tag: "SimpleSIntMessage == 65535",
                        field: sint32Msg.testFieldInt = 65535, answer: 65535 },
                    {tag: "SimpleSIntMessage == -2147483648",
                        field: sint32Msg.testFieldInt = -2147483648, answer: -2147483648 },
                    {tag: "SimpleSIntMessage == 2147483647",
                        field: sint32Msg.testFieldInt = 2147483647, answer: 2147483647 },
                ]
    }

    function test_simplesintmessage(data) {
        compare(data.field, data.answer)
    }

    function test_simpleuintmessage_data() {
        return [
                    {tag: "SimpleUIntMessage == 0",
                        field: uint32Msg.testFieldInt = 0, answer: 0 },
                    {tag: "SimpleUIntMessage == 127",
                        field: uint32Msg.testFieldInt = 127, answer: 127 },
                    {tag: "SimpleUIntMessage == 255",
                        field: uint32Msg.testFieldInt = 255, answer: 255 },
                    {tag: "SimpleUIntMessage == 32767",
                        field: uint32Msg.testFieldInt = 32767, answer: 32767 },
                    {tag: "SimpleUIntMessage == 65535",
                        field: uint32Msg.testFieldInt = 65535, answer: 65535 },
                    {tag: "SimpleUIntMessage == -2147483648",
                        field: uint32Msg.testFieldInt = 2147483647, answer: 2147483647 },
                    {tag: "SimpleUIntMessage == 4294967295",
                        field: uint32Msg.testFieldInt = 4294967295, answer: 4294967295 },
                ]
    }

    function test_simpleuintmessage(data) {
        compare(data.field, data.answer)
    }

    function test_simplefixed32message_data() {
        return [
                    {tag: "SimpleFixedInt32Message == 0",
                        field: fixed32Msg.testFieldInt = 0, answer: 0 },
                    {tag: "SimpleFixedInt32Message == 127",
                        field: fixed32Msg.testFieldInt = 127, answer: 127 },
                    {tag: "SimpleFixedInt32Message == 255",
                        field: fixed32Msg.testFieldInt = 255, answer: 255 },
                    {tag: "SimpleFixedInt32Message == 32767",
                        field: fixed32Msg.testFieldInt = 32767, answer: 32767 },
                    {tag: "SimpleFixedInt32Message == 65535",
                        field: fixed32Msg.testFieldInt = 65535, answer: 65535 },
                    {tag: "SimpleFixedInt32Message == -2147483648",
                        field: fixed32Msg.testFieldInt = 2147483647, answer: 2147483647 },
                    {tag: "SimpleFixedInt32Message == 4294967295",
                        field: fixed32Msg.testFieldInt = 4294967295, answer: 4294967295 },
                ]
    }

    function test_simplefixed32message(data) {
        compare(data.field, data.answer)
    }

    function test_simplesfixed32message_data() {
        return [
                    {tag: "SimpleSFixedInt32Message == 0",
                        field: sfixed32Msg.testFieldInt = 0, answer: 0 },
                    {tag: "SimpleSFixedInt32Message == -128",
                        field: sfixed32Msg.testFieldInt = -128, answer: -128 },
                    {tag: "SimpleSFixedInt32Message == 127",
                        field: sfixed32Msg.testFieldInt = 127, answer: 127 },
                    {tag: "SimpleSFixedInt32Message == -256",
                        field: sfixed32Msg.testFieldInt = -256, answer: -256 },
                    {tag: "SimpleSFixedInt32Message == 255",
                        field: sfixed32Msg.testFieldInt = 255, answer: 255 },
                    {tag: "SimpleSFixedInt32Message == -32768",
                        field: sfixed32Msg.testFieldInt = -32768, answer: -32768 },
                    {tag: "SimpleSFixedInt32Message == 32767",
                        field: sfixed32Msg.testFieldInt = 32767, answer: 32767 },
                    {tag: "SimpleSFixedInt32Message == -65536",
                        field: sfixed32Msg.testFieldInt = -65536, answer: -65536 },
                    {tag: "SimpleSFixedInt32Message == 65535",
                        field: sfixed32Msg.testFieldInt = 65535, answer: 65535 },
                    {tag: "SimpleSFixedInt32Message == -2147483648",
                        field: sfixed32Msg.testFieldInt = -2147483648, answer: -2147483648 },
                    {tag: "SimpleSFixedInt32Message == 2147483647",
                        field: sfixed32Msg.testFieldInt = 2147483647, answer: 2147483647 },
                ]
    }
    function test_simplesfixed32message(data) {
        compare(data.field, data.answer)
    }

    function test_simplesstringmessage() {
        compare(stringMsg.testFieldString, "Test string", "SimpleStringMessage")
    }

    function test_int32ImplicitConversion_data() {
        return [
                    {tag: "int32ImplicitConversion int32Msg == 0",
                        field: int32Msg.testFieldInt = 0, answer: false },
                    {tag: "int32ImplicitConversion int32Msg == 1",
                        field: int32Msg.testFieldInt = 1, answer: true },
                ]
    }

    function test_int32ImplicitConversion(data) {
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be false")
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be true")
    }

    function test_int32LocaleStringConversion() {
        compare(int32Msg.testFieldInt.toLocaleString(Qt.locale()),
                Number(int32Msg.testFieldInt).toLocaleString(Qt.locale()),
                "Locale number string is not match "
                + int32Msg.testFieldInt.toLocaleString(Qt.locale())
                + " != " + Number(int32Msg.testFieldInt).toLocaleString(Qt.locale()))
    }

    function test_fixed32ImplicitConversion_data() {
        return [
                    {tag: "fixed32ImplicitConversion fixed32Msg == 0",
                        field: fixed32Msg.testFieldFixedInt32 = 0, answer: false },
                    {tag: "fixed32ImplicitConversion fixed32Msg == 1",
                        field: fixed32Msg.testFieldFixedInt32 = 1, answer: true },
                ]
    }

    function test_fixed32ImplicitConversion(data) {
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be false")
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be true")
    }

    function test_fixed32LocaleStringConversion() {
        compare(fixed32Msg.testFieldFixedInt32.toLocaleString(Qt.locale()),
                Number(fixed32Msg.testFieldFixedInt32).toLocaleString(Qt.locale()),
                "Locale number string is not match "
                + fixed32Msg.testFieldFixedInt32.toLocaleString(Qt.locale())
                + " != " + Number(fixed32Msg.testFieldFixedInt32).toLocaleString(Qt.locale()))
    }

    function test_sint32ImplicitConversion_data() {
        return [
                    {tag: "sint32ImplicitConversion testFieldInt == 0",
                        field: fixed32Msg.testFieldInt = 0, answer: false },
                    {tag: "sint32ImplicitConversion testFieldInt == 1",
                        field: fixed32Msg.testFieldInt = 1, answer: true },
                ]
    }

    function test_sint32ImplicitConversion(data) {
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be false")
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be true")
    }

    function test_sint32LocaleStringConversion() {
        compare(sint32Msg.testFieldInt.toLocaleString(Qt.locale()),
                Number(sint32Msg.testFieldInt).toLocaleString(Qt.locale()),
                "Locale number string is not match "
                + sint32Msg.testFieldInt.toLocaleString(Qt.locale())
                + " != " + Number(sint32Msg.testFieldInt).toLocaleString(Qt.locale()))
    }

    function test_sfixed32ImplicitConversion_data() {
        return [
                    {tag: "sfixed32ImplicitConversion sfixed32Msg == 0",
                        field: sfixed32Msg.testFieldInt = 0, answer: false },
                    {tag: "sfixed32ImplicitConversion sfixed32Msg == 1",
                        field: sfixed32Msg.testFieldInt = 1, answer: true },
                ]
    }

    function test_sfixed32ImplicitConversion(data) {
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be false")
        compare(data.field ? true : false, data.answer,
                "Invalid implicit conversion: " + data.field + " should be true")
    }

    function test_sfixed32LocaleStringConversion() {
        compare(sfixed32Msg.testFieldFixedInt32.toLocaleString(Qt.locale()),
                Number(sfixed32Msg.testFieldFixedInt32).toLocaleString(Qt.locale()),
                "Locale number string is not match "
                + sfixed32Msg.testFieldFixedInt32.toLocaleString(Qt.locale())
                + " != " + Number(sfixed32Msg.testFieldFixedInt32).toLocaleString(Qt.locale()))
    }

    function test_complexMessage_data() {
        return [
                    {tag: "inner text",
                        field: complexMsg.testComplexField.testFieldString, answer: "inner" },
                    {tag: "My test text",
                        field: complexMsg.testComplexField.testFieldString = "My test",
                        answer: "My test" },
                    {tag: "outer massage",
                        field: complexMsg.testComplexField = outerMessage, answer: outerMessage },
                    {tag: "inner massage",
                        field: complexMsg.testComplexField = innerMessage, answer: innerMessage },
                    {tag: "innerMessage text",
                        field: complexMsg.testComplexField.testFieldString, answer: "inner" },
                ]
    }

    function test_complexMessage(data) {
        compare(data.field, data.answer)
    }
}
