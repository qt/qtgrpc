// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

import QtQuick
import QtTest
import QtGrpcQuick
import QmlTestUri

TestCase {
    id: root
    name: "QtGrpcHttp2Channel QML registration test"

    GrpcChannelOptions {
        id: optionsVar
        host: "http://localhost:50051"
        deadline: { 1000 }
        metadata:  GrpcMetadata {
            id: grpcData
            data: ({ "user-name": "localhost",
                     "user-password": "qwerty"})
        }
    }

    GrpcHttp2Channel {
        id: channelId
        options: GrpcChannelOptions {
            host: "http://localhost:50051"
            deadline: { 1000 }
            metadata: grpcData
        }
    }

    GrpcHttp2Channel {
        id: secondChannelId
        options: optionsVar
    }

    function test_2GrpcTypes_data() {
        return [
                    { tag: "channelId is an object",
                        field: typeof channelId, answer: "object" },
                    { tag: "secondChannelId is an object",
                        field: typeof secondChannelId, answer: "object" },
                    { tag: "channelId.options is an object",
                        field: typeof channelId.options, answer: "object" },
                    { tag: "optionsVar is an object",
                        field: typeof optionsVar, answer: "object" }
                ]
    }

    function test_2GrpcTypes(data) {
        compare(data.field, data.answer)
    }

    function test_3GrpcChannelValues_data() {
        return [
                    { tag: "channelId Host",
                        field: channelId.options.host, answer: "http://localhost:50051" },
                    { tag: "channelId deadline",
                        field: channelId.options.deadline, answer: 1000 },
                    { tag: "channelId metadata",
                        field: channelId.options.metadata, answer: grpcData },
                    { tag: "secondChannelId deadline",
                        field: secondChannelId.options.deadline, answer: 1000 },
                    { tag: "secondChannelId Host",
                        field: secondChannelId.options.host, answer: "http://localhost:50051" },
                    { tag: "secondChannelId metadata",
                        field: secondChannelId.options.metadata, answer: grpcData },
                    { tag: "secondChannelId metadata == channelId metadata",
                        field: secondChannelId.options.metadata,
                        answer: channelId.options.metadata }
                ]
    }

    function test_3GrpcChannelValues(data) {
        compare(data.field, data.answer)
    }
}
