// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick

Item {
    id: root
    property int currentBarValue: 0
    property int totalBarValue: 0

    property string activeColor: ""
    property string bgColor: ""

    width: 300
    height: 10

    Rectangle {
        id: totalValue
        anchors.centerIn: parent
        width: 300
        height: 10
        radius: 80

        color: root.bgColor
    }

    Rectangle {
        anchors.left: parent.left
        width: (root.currentBarValue && root.totalBarValue > 0)
               ? totalValue.width * (root.currentBarValue/root.totalBarValue)
               : 0
        height: 10
        radius: 80
        color: root.activeColor
    }
}
