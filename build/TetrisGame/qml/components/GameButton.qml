import QtQuick

// ============================================================
//  GameButton.qml — 主题按钮 / Themed action button
//  支持 primary / secondary 样式 + 悬停/按压状态
//  Primary/secondary styles with hover/press states
// ============================================================
Rectangle {
    id: btn

    property string label: ""
    property bool   primary: false
    signal clicked()

    height: 36
    radius: 8

    color: {
        if (!enabled) return Qt.rgba(1, 1, 1, 0.03);
        if (pressed)  return Qt.rgba(1, 1, 1, 0.15);
        if (primary)  return "#ee5a24";
        return Qt.rgba(1, 1, 1, 0.06);
    }
    border {
        color: enabled ? (primary ? "#ff6348" : Qt.rgba(1, 1, 1, 0.1))
                       : Qt.rgba(1, 1, 1, 0.03);
        width: 1
    }

    property bool pressed: false

    Text {
        anchors.centerIn: parent
        font.pixelSize: 13
        font.weight: primary ? Font.Bold : Font.Normal
        color: !enabled ? "#555" : primary ? "#fff" : "#ccc"
        text: label
    }

    MouseArea {
        anchors.fill: parent
        enabled: btn.enabled
        hoverEnabled: true
        onPressed:  btn.pressed = true
        onReleased: btn.pressed = false
        onCanceled: btn.pressed = false
        onClicked:  btn.clicked()
        cursorShape: Qt.PointingHandCursor
    }

    Behavior on color { ColorAnimation { duration: 120 } }
}
