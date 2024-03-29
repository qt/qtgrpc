// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtQuick
import QtTest
import QtGrpc
import QmlTestUri
import QtNetwork

Item {
    id: root

    property bool sslSupport: options.sslConfiguration !== undefined

    property Item sslDynamicItem;

    function createSslConfigurationItem() {
        return Qt.createQmlObject("import QtQuick; import QtGrpc; import QtNetwork; Item { \
                                   property sslConfiguration defaultConfig; }", root)
    }

    Component.onCompleted: {
        if (sslSupport) {
            sslDynamicItem = root.createSslConfigurationItem()
            options.sslConfiguration = sslDynamicItem.defaultConfig
        }
        optionsWithChangedProperties.metadata = null
        optionsWithChangedProperties.deadline = 3000
        options.serializationFormat = GrpcSerializationFormat.Json

    }

    GrpcChannelOptions {
        id: options
        host: "http://localhost:50051"
        deadline: { 1000 }
        metadata: GrpcMetadata {
            id: grpcData
            data: ({ "user-name": "localhost",
                     "user-password": "qwerty"})
        }
        serializationFormat: {GrpcSerializationFormat.Protobuf}
    }

    GrpcChannelOptions {
        id: optionsWithChangedProperties
        host: "http://localhost:50051"
        metadata: GrpcMetadata {
            data: ({ "user-name": "localhost",
                       "user-password": "qwerty"})
        }
        deadline: { 1000 }
        serializationFormat: {GrpcSerializationFormat.Json}
    }

    TestCase {
        id: optionsCase
        name: "qtgrpcSslChannelOptionsTest"

        function test_1initialization()
        {
            if (!sslSupport) {
                warn("No SSL support detected!")
                compare(options.sslConfiguration, undefined)
            }
        }

        function test_sslOptionType_data() {
            if (sslSupport) {
                return [
                            { tag: "peerVerifyMode == AutoVerifyPeer",
                                field: options.sslConfiguration.peerVerifyMode , answer: SslSocket.AutoVerifyPeer }
                        ]
            } else {
                return [
                            { tag: "SSL option is not creatable",
                                field: options.sslConfiguration, answer: undefined }
                        ]
            }
        }

        function test_sslOptionType(data) {
            compare(data.field, data.answer)
        }
    }

    TestCase {
        name: "qtgrpcChannelOptionsTest"
        function test_OptionTypes_data() {
            return [
                        { tag: "options is an object",
                            field: typeof options, answer: "object" },
                        { tag: "options.metadata is an object",
                            field: typeof options.metadata, answer: "object" },
                        { tag: "options.metadata.data is an object",
                            field: typeof options.metadata.data, answer: "object" },
                        { tag: "grpcData is an object",
                            field: typeof grpcData, answer: "object" },
                        { tag: "GrpcSerializationFormat.Protobuf == 1",
                            field: GrpcSerializationFormat.Protobuf, answer: 1 },
                        { tag: "GrpcSerializationFormat.Json == 2",
                            field: GrpcSerializationFormat.Json, answer: 2 }
                    ]
        }

        function test_OptionTypes(data) {
            compare(data.field, data.answer)
        }

        function test_ChannelOptions_data() {
            return [
                        { tag: "options URL is set",
                            field: options.host, answer: "http://localhost:50051" },
                        { tag: "options.metadata == grpcData",
                            field: options.metadata, answer: grpcData },
                        { tag: "options.metadata.data == grpcData.data",
                            field: options.metadata.data, answer: grpcData.data },
                        { tag: "options.metadata.data[user-name] == localhost",
                            field: options.metadata.data["user-name"], answer: "localhost" },
                        { tag: "options.metadata.data[user-password] == qwerty",
                            field: options.metadata.data["user-password"], answer: "qwerty" },
                        { tag: "metadata deadline = 1000",
                            field: options.deadline, answer: 1000 },
                        { tag: "optionsWithChangedProperties.metadata == null",
                            field: optionsWithChangedProperties.metadata, answer: null },
                        { tag: "optionsWithChangedProperties.deadline == 3000",
                            field: optionsWithChangedProperties.deadline, answer: 3000 }
                    ]
        }

        function test_ChannelOptions(data) {
            compare(data.field, data.answer)
        }

        function test_serializationFormat_data() {
            return [
                        { tag: "options.serializationFormat == GrpcSerializationFormat.Json",
                            field: options.serializationFormat,
                            answer: GrpcSerializationFormat.Json },
                        { tag: "qtgrpcSslChannelOptionsTest.serializationFormat == GrpcSerializationFormat.Json",
                            field: optionsWithChangedProperties.serializationFormat,
                            answer: GrpcSerializationFormat.Json },
                        { tag: "optionsWithChangedProperties.metadata == null",
                            field: optionsWithChangedProperties.metadata, answer: null },
                        { tag: "optionsWithChangedProperties.deadline == 3000",
                            field: optionsWithChangedProperties.deadline, answer: 3000 }
                    ]
        }

        function test_serializationFormat(data) {
            compare(data.field, data.answer)
        }
    }
}
