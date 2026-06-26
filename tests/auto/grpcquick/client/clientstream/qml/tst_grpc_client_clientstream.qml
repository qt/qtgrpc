// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtTest
import GrpcQuickTest.Client.ClientStream
import QtGrpc;
import qtgrpc.tests

Item {
    id: root

    readonly property int expectedNumberOfMessages: 4

    property simpleStringMessage messageArg;
    property string result: ""
    property var streamSender: null
    property bool errorCallbackCalled: false
    property int times: 1

    Timer {
        running: root.streamSender != null && root.times < root.expectedNumberOfMessages
        interval: testMessageLatency
        onTriggered: {
            root.streamSender.writeMessage(root.messageArg)
            ++times
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
        name: "clientStream"

        property bool done: false

        function test_clientStream() {
            root.messageArg.testFieldString = "streamQml"
            root.streamSender = clientQml.testMethodClientStream(
                root.messageArg,
                function(msg) {
                    result = msg.testFieldString
                    testCase.done = true
                },
                function(status) {
                    root.errorCallbackCalled = true
                    testCase.done = true
                })
            tryVerify(function() { return testCase.done },
                      testMessageLatencyWithThreshold * root.expectedNumberOfMessages + 1000,
                      "Client stream did not complete in time")
            compare(root.result, "streamQml1streamQml2streamQml3streamQml4")
            compare(root.times, root.expectedNumberOfMessages)
            verify(!root.errorCallbackCalled)
        }
    }

    Component.onCompleted: {
        root.messageArg.testFieldString = "streamQml"
    }
}
