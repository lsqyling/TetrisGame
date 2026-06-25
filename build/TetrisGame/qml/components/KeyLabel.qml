import QtQuick

// ============================================================
//  KeyLabel.qml — 键盘快捷键徽章 / Keyboard shortcut badge
// ============================================================
Rectangle {
    property string text

    width: 42; height: 20
    color: Qt.rgba(1, 1, 1, 0.07)
    border { color: Qt.rgba(1, 1, 1, 0.1); width: 1 }
    radius: 4

    Text {
        anchors.centerIn: parent
        font.pixelSize: 11
        color: "#ccc"
        text: parent.text
    }
}
