// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import qtgrpc.examples.vehicle

ApplicationWindow {
    id: root
    width: 1280
    height: 518

    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height

    property int speed: 0
    property int rpm: 0
    property int totalDistance: 0
    property int remainingDistance: 0
    property int direction: ClusterDataManager.BACKWARD
    property int fuelLevel: 0
    property bool availableBtn: false

    visible: true
    title: qsTr("Cluster Qt GRPC Example")
    Material.theme: Material.Light

    Rectangle {
        anchors.fill: parent
        color: "#040a16"
    }

    Item {
        id: background
        anchors.fill: parent
        visible: !root.availableBtn

        Row {
            id: textsBar
            anchors.horizontalCenter: background.horizontalCenter
            anchors.bottom: progressBars.top
            spacing: 130

            width: root.width - 110
            height: 200

            Item {
                width: 300
                height: 200

                ClusterText {
                    width: 200
                    height: 200
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left

                    verticalAlignment: Text.AlignBottom
                    font.pointSize: 90
                    text: root.speed
                }

                ClusterText {
                    width: 100
                    height: 200
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 27
                    anchors.right: parent.right

                    verticalAlignment: Text.AlignBottom
                    horizontalAlignment: Text.AlignRight
                    text: "Kmph"
                }
            }

            Item {
                width: 300
                height: 200

                Image {
                    id: arrow
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 33
                    source: root.getImage()
                    width: implicitWidth
                    height: implicitHeight

                    Timer {
                        interval: 2000
                        running: arrow.source !== ""
                        repeat: true
                        onTriggered: arrow.visible = !arrow.visible
                    }
                }

                ClusterText {
                    width: 200
                    height: 40
                    anchors.bottom: street.top
                    anchors.right: parent.right

                    verticalAlignment: Text.AlignBottom
                    horizontalAlignment: Text.AlignRight
                    text: Number(root.remainingDistance * 0.001).toFixed(2) + " km"
                }

                ClusterText {
                    id: street
                    width: 200
                    height: 40
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 27
                    anchors.right: parent.right

                    verticalAlignment: Text.AlignBottom
                    horizontalAlignment: Text.AlignRight
                    color: "#828284"
                    text: (root.getImage() !== "") ? "Erich-Thilo St" : "None"
                }
            }

            Item {
                width: 300
                height: 200

                ClusterText {
                    id: rightSide
                    width: 200
                    height: 200
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 27
                    anchors.right: parent.right

                    verticalAlignment: Text.AlignBottom
                    horizontalAlignment: Text.AlignRight
                    text: root.rpm + " rpm"
                }
            }
        }

        Row {
            id: progressBars

            anchors.horizontalCenter: background.horizontalCenter
            anchors.verticalCenter: background.verticalCenter
            anchors.verticalCenterOffset: 50
            spacing: 130

            width: root.width - 110
            height: 20

            ClusterProgressBar {
                currentBarValue: root.speed;
                totalBarValue: 200
                activeColor: "#04e2ed"
                bgColor: "#023061"
            }
            ClusterProgressBar {
                currentBarValue: root.remainingDistance;
                totalBarValue: root.totalDistance
                activeColor: "#04e2ed"
                bgColor: "#023061"
            }
            ClusterProgressBar {
                currentBarValue: root.rpm;
                totalBarValue: 9000
                activeColor: "#f8c607"
                bgColor: "#5f3f04"
            }
        }

        ClusterProgressBar {
            id: fuelLevel
            anchors.leftMargin: 55
            anchors.left: parent.left
            anchors.topMargin: 100
            anchors.top: progressBars.bottom
            currentBarValue: root.fuelLevel
            totalBarValue: 250
            activeColor: "#05c848"
            bgColor: "#03511f"
        }

        ClusterText {
            id: miles
            anchors.bottom: fuelLevel.bottom
            anchors.bottomMargin: 27
            anchors.right: fuelLevel.right

            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignRight
            text: root.fuelLevel + " Km"
        }

        Image {
            anchors.bottom: fuelLevel.bottom
            anchors.bottomMargin: 35
            anchors.left: fuelLevel.left

            source:"qrc:/fuel_lvl.png"
        }
    }

    Item {
        anchors.fill: parent
        visible: root.availableBtn
        ClusterText {
            anchors.top: parent.top
            anchors.topMargin: 60
            anchors.right: restartBtn.left
            anchors.rightMargin: 10

            width: 200
            height: 80
            font.pointSize: 14
            visible: root.availableBtn
            text: "Please, start server and press run."
        }

        Rectangle {
            id: restartBtn
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: 60

            width: 100
            height: 50
            radius: 80
            color: "#040a16"
            border.color: btn.btnPressed ? "#828284" : "white"
            border.width: 2
            visible: root.availableBtn

            ClusterText {
                anchors.centerIn: parent
                color: btn.btnPressed ? "#828284" : "white"
                text: "RUN"
            }

            MouseArea {
                id: btn
                property bool btnPressed: false
                anchors.fill: parent

                enabled: root.availableBtn
                onClicked: ClusterDataManager.restart()
                onPressed: btnPressed = true
                onReleased:  btnPressed = false
            }
        }
    }

    function getImage() {
        switch (root.direction) {
        case ClusterDataManager.RIGHT:
            return "qrc:/right.png"
        case ClusterDataManager.LEFT:
            return "qrc:/left.png"
        case ClusterDataManager.STRAIGHT:
            return "qrc:/forward.png"
        default:
            return ""
        }
    }

    Connections {
        target: ClusterDataManager

        function onTotalDistanceChanged(distance) {
            root.totalDistance = distance
        }

        function onSpeedChanged(speed) {
            root.speed = speed
        }

        function onRpmChanged(rpm) {
            root.rpm = rpm
        }

        function onRemainingDistanceChanged(distance) {
            root.remainingDistance = distance
        }

        function onDirectionChanged(direction) {
            root.direction = direction
        }

        function onFuelLevelChanged(level) {
            root.fuelLevel = level
        }

        function onShowRestartClient(value) {
            root.availableBtn = value
        }
    }
}
