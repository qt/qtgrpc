// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtGrpc;
import QtProtobuf;
import QtTest
import GrpcQuickTest.Client.UnaryCall
import qtgrpc.tests.unarycall

Item {
    id: root

    property simpleStringMessage messageArg;
    property simpleStringMessage messageResponse;

    property var clientQml
    property var grpcChannel
    property var grpcChannelDeadline

    function createClientItem() {
        return Qt.createQmlObject("import QtQuick; import QtGrpc; \
                                   import qtgrpc.tests.unarycall; TestServiceClient {}", root)
    }

    function createGrpcChannelItem() {
        return Qt.createQmlObject("import QtQuick; import QtGrpc; GrpcHttp2Channel { \
                                   hostUri: \"http://localhost:50051\"; \
                                   options: GrpcChannelOptions { \
                                   deadlineTimeout: { 2000 } \
                                   metadata: GrpcMetadata {
                                        data: ({ \"common-meta-data\": \"test-channel-metadata\" }) \
                                   }}}", root)
    }

    function createGrpcChannelWithDeadlineItem() {
        return Qt.createQmlObject("import QtQuick; import QtGrpc; GrpcHttp2Channel { \
                                   hostUri: \"http://localhost:50051\"; \
                                   options: GrpcChannelOptions { \
                                   deadlineTimeout: { 1000 } }  }", root)
    }

    TestCase {
        id: registrationTestCase
        name: "qtgrpcClientRegistration"

        property bool callDone: false
        property bool callErrorOccurred: false

        function test_1clientTypes_data() {
            return [
                        { tag: "Grpc Client created",
                            field: typeof clientQml, answer: "object" },
                        { tag: "Grpc Http2 Channel created",
                            field: typeof grpcChannel, answer: "object" },
                        { tag: "Grpc Http2 Deadline Channel created",
                            field: typeof grpcChannelDeadline, answer: "object" }
                    ]
        }

        function test_1clientTypes(data) {
            compare(data.field, data.answer)
        }

        function test_ChannelOptions_data() {
            return [
                        { tag: "grpcChannelOptions URL is set",
                            field: grpcChannel.hostUri, answer: "http://localhost:50051" },
                        { tag: "grpcChannelOptions deadline is set",
                            field: grpcChannelDeadline.options.deadlineTimeout, answer: 1000 }
                    ]
        }

        function test_ChannelOptions(data) {
            compare(data.field, data.answer)
        }

        function test_testMethodCall() {
            registrationTestCase.callDone = false
            registrationTestCase.callErrorOccurred = false
            clientQml.testMethod(root.messageArg,
                                 function(value) {
                                     root.messageResponse = value
                                     registrationTestCase.callDone = true
                                 },
                                 function(status) {
                                     console.log("testMethod error: " + status.code
                                                 + " - " + status.message)
                                     registrationTestCase.callErrorOccurred = true
                                     registrationTestCase.callDone = true
                                 })
            tryVerify(function() { return registrationTestCase.callDone },
                      testMessageLatencyWithThreshold + 1000,
                      "testMethod call did not complete in time")
            verify(root.messageResponse == root.messageArg)
            verify(!registrationTestCase.callErrorOccurred)
        }
    }

    TestCase {
        id: unaryCallWithOptions
        name: "unaryCallWithOptions"
        property empty arg;
        property metadataMessage result;
        property bool errorOccurred: false;
        property bool done: false;

        GrpcCallOptions {
            id: options
            metadata: GrpcMetadata {
                data: ({ "user-name": "localhost",
                         "user-password": "qwerty"})
            }
        }

        function removeElementFromArray(array, element) {
            var index = array.indexOf(element)
            verify(index !== -1)
            array.splice(index, 1)
        }

        function test_unaryCallWithOptions() {
            unaryCallWithOptions.done = false
            unaryCallWithOptions.errorOccurred = false
            clientQml.replyWithMetadata(unaryCallWithOptions.arg,
                                        function(value) {
                                            unaryCallWithOptions.result = value
                                            unaryCallWithOptions.done = true
                                        },
                                        function(status) {
                                            console.log("replyWithMetadata error: " + status.code
                                                        + " - " + status.message)
                                            unaryCallWithOptions.errorOccurred = true
                                            unaryCallWithOptions.done = true
                                        },
                                        options)
            tryVerify(function() { return unaryCallWithOptions.done },
                      testMessageLatencyWithThreshold + 1000,
                      "replyWithMetadata call did not complete in time")
            verify(!unaryCallWithOptions.errorOccurred, "unaryCallWithOptions ended with error")

            var missingHeaders = Array()
            missingHeaders.push("user-name")
            missingHeaders.push("user-password")
            missingHeaders.push("common-meta-data")

            for (var i = 0; i < unaryCallWithOptions.result.valuesData.length; i++) {
                var md = unaryCallWithOptions.result.valuesData[i]
                if (md.key === "user-name" && md.value === "localhost")
                    removeElementFromArray(missingHeaders, "user-name")
                if (md.key === "user-password" && md.value === "qwerty")
                    removeElementFromArray(missingHeaders, "user-password")
                if (md.key === "common-meta-data" && md.value === "test-channel-metadata")
                    removeElementFromArray(missingHeaders, "common-meta-data")
            }

            verify(missingHeaders.length === 0,
                   "Missing headers from server: " + missingHeaders)
        }
    }

    Component.onCompleted: {
        clientQml = root.createClientItem()
        grpcChannel = root.createGrpcChannelItem()
        grpcChannelDeadline = root.createGrpcChannelWithDeadlineItem()
        clientQml.channel = grpcChannel.channel
    }
}
