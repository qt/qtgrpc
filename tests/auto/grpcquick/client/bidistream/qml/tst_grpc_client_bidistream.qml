// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtTest
import GrpcQuickTest.Client.BidiStream
import QtGrpc;
import qtgrpc.tests.bidistream

Item {
    id: root

    readonly property int expectedNumberOfMessages: 4

    property simpleStringMessage messageArg;
    property string result: ""
    property var streamSender: null
    property bool errorCallbackCalled: false
    property int times: 1

    function readMessage(msg) {
        root.result += msg.testFieldString

        if (root.times < expectedNumberOfMessages) {
            ++root.times
            root.messageArg.testFieldString = "streamQml" + root.times
            testCase.verify(root.streamSender,
                            "readMessage callback is called without active stream sender")
            root.streamSender.writeMessage(root.messageArg)
        }
    }

    GrpcHttp2Channel {
        id: httpChannel
        hostUri: "http://localhost:50051"
        options: GrpcChannelOptions {
        }
    }

    TestServiceClient {
        id: clientQml
        channel: httpChannel.channel
    }

    TestCase {
        id: testCase
        name: "bidiStream"

        property bool done: false

        function test_bidiStream() {
            root.messageArg.testFieldString = "streamQml" + root.times
            root.streamSender = clientQml.testMethodBiStream(
                root.messageArg,
                root.readMessage,
                function() { testCase.done = true },
                function(status) {
                    root.errorCallbackCalled = true
                    testCase.done = true
                })
            tryVerify(function() { return testCase.done },
                      testMessageLatencyWithThreshold * root.expectedNumberOfMessages + 1000,
                      "Bidirectional stream did not complete in time")
            compare(root.result, "streamQml11streamQml22streamQml33streamQml44")
            compare(root.times, root.expectedNumberOfMessages)
            verify(!root.errorCallbackCalled)
        }
    }
}
