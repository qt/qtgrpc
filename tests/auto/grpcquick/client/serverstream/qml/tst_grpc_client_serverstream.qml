// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtTest
import GrpcQuickTest.Client.ServerStream
import QtGrpc;
import qtgrpc.tests.serverstream

Item {
    id: root

    property simpleStringMessage messageArg;
    property string result: ""

    property bool errorCallbackCalled: false
    property int times: 0

    readonly property int expectedNumberOfMessages: 4

    function readMessage(msg) {
        root.result += msg.testFieldString
        ++root.times
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
        name: "serverStream"

        property bool done: false

        function test_serverStream() {
            root.messageArg.testFieldString = "streamQml"
            clientQml.testMethodServerStream(root.messageArg,
                root.readMessage,
                function() { testCase.done = true },
                function(status) {
                    root.errorCallbackCalled = true
                    testCase.done = true
                })
            tryVerify(function() { return testCase.done },
                      testMessageLatencyWithThreshold * (root.expectedNumberOfMessages + 1) + 1000,
                      "Server stream did not complete in time")
            compare(root.result, "streamQml1streamQml2streamQml3streamQml4")
            compare(root.times, expectedNumberOfMessages)
            verify(!root.errorCallbackCalled)
        }
    }
}
