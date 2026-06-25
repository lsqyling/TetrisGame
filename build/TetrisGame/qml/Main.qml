import QtQuick
import QtQuick.Window
import TetrisConfig
import GameBackend 1.0

// ============================================================
//  Main.qml — 根窗口 / Root Window
//  无边框、浮动按钮、全窗口拖拽、键盘处理、音频连接
//  Frameless window, auto-hide buttons, drag, keyboard, audio
// ============================================================
Window {
    id: root
    visible: true
    title: qsTr("俄罗斯方块 — Tetris")
    flags: Qt.FramelessWindowHint | Qt.Window

    // 固定尺寸窗口 / Fixed-size window
    width:  TetrisConfig.windowW
    height: TetrisConfig.windowH
    minimumWidth:  width
    maximumWidth:  width
    minimumHeight: height
    maximumHeight: height
    color: TetrisConfig.bgColor

    // ============================================================
    //  浮动窗口按钮 (z: 10 — 右上角) / Floating window buttons
    //  默认隐藏，悬停淡入，离开淡出
    // ============================================================
    Row {
        id: windowButtons
        z: 10
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 4
        anchors.rightMargin: 6
        spacing: 6

        // 最小化按钮 / Minimize button — 半透明悬停
        Rectangle {
            id: minBtnBg
            width: 30
            height: 22
            radius: 4
            opacity: minBtnHover.containsMouse ? 1 : 0
            color: Qt.rgba(1, 1, 1, 0.12)

            Behavior on opacity { NumberAnimation { duration: 150 } }

            Text {
                anchors.centerIn: parent
                text: "—"
                color: Qt.rgba(1, 1, 1, minBtnHover.containsMouse ? 0.8 : 0.0)
                font.pixelSize: 13
            }
            MouseArea {
                id: minBtnHover
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.showMinimized()
            }
        }

        // 关闭按钮 / Close button — 橘红色悬停 / orange-red hover
        Rectangle {
            id: closeBtnBg
            width: 30
            height: 22
            radius: 4
            opacity: closeBtnHover.containsMouse ? 1 : 0
            color: "#e81123"

            Behavior on opacity { NumberAnimation { duration: 150 } }

            Text {
                anchors.centerIn: parent
                text: "✕"
                color: Qt.rgba(1, 1, 1, closeBtnHover.containsMouse ? 1 : 0.0)
                font.pixelSize: 11
            }
            MouseArea {
                id: closeBtnHover
                anchors.fill: parent
                hoverEnabled: true
                onClicked: root.close()
            }
        }
    }

    // ============================================================
    //  内容区域 / Content area
    // ============================================================
    Item {
        id: contentItem
        anchors.fill: parent
        focus: true

        Component.onCompleted: contentItem.forceActiveFocus()

        // 粒子动画背景（z:0）/ Animated background
        BackgroundEffect {
            anchors.fill: parent
            z: 0
        }

        // 全窗口拖拽 (z:0，位于 GamePage 后方)
        // 空白区域 startSystemMove()，按钮不受影响
        // Full-window drag — below GamePage, buttons unaffected
        MouseArea {
            anchors.fill: parent
            z: 0
            acceptedButtons: Qt.LeftButton
            onPressed: root.startSystemMove()
        }

        // 游戏主界面 (z:1) / Game content
        GamePage {
            id: gamePage
            anchors.fill: parent
            z: 1
        }

        // ============================================================
        //  键盘控制 / Keyboard controls
        // ============================================================
        Keys.onPressed: (event) => {
            switch (event.key) {
            case Qt.Key_J:
            case Qt.Key_Down:
                GameEngine.moveDown();
                event.accepted = true;
                break;
            case Qt.Key_H:
            case Qt.Key_Left:
                GameEngine.moveLeft();
                event.accepted = true;
                break;
            case Qt.Key_L:
            case Qt.Key_Right:
                GameEngine.moveRight();
                event.accepted = true;
                break;
            case Qt.Key_Space:
                GameEngine.rotatePiece();
                event.accepted = true;
                break;
            case Qt.Key_Return:
            case Qt.Key_Enter:
                event.accepted = true;
                if (GameEngine.gameOver || !GameEngine.gameRunning) {
                    AudioEngine.startMusic();
                    GameEngine.startGame();
                    gamePage.gameOverlay.opacity = 0;
                } else {
                    GameEngine.hardDrop();
                }
                break;
            case Qt.Key_P:
                GameEngine.togglePause();
                event.accepted = true;
                break;
            case Qt.Key_M:
                AudioEngine.toggleMusic();
                event.accepted = true;
                break;
            }
        }
    }

    // ============================================================
    //  音频事件连接 / Audio wiring
    // ============================================================
    Connections {
        target: GameEngine
        function onLinesCleared(count) {
            AudioEngine.playClear(count);
        }
    }

    Connections {
        target: GameEngine
        function onStateChanged() {
            if (GameEngine.gameOver) {
                AudioEngine.playGameOver();
                AudioEngine.stopMusic();
            } else if (GameEngine.paused) {
                AudioEngine.pauseMusic();
            } else if (GameEngine.gameRunning) {
                AudioEngine.resumeMusic();
            }
        }
    }
}
