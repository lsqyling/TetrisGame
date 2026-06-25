import QtQuick
import QtQuick.Window
import TetrisConfig
import GameBackend 1.0

// ============================================================
//  GamePage.qml — 三栏主布局 / Three-column main layout
//  左栏：下一方块预览 + 操作说明
//  中栏：游戏棋盘 + 覆盖层
//  右栏：分数面板 + 按钮
// ============================================================
Item {
    id: page

    property alias gameOverlay: gameOverlay

    // ============================================================
    //  标题 / Title
    // ============================================================
    Column {
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 34
            font.weight: Font.Black
            font.family: "Arial Black"
            text: qsTr("俄罗斯方块")
            color: TetrisConfig.titleColor
            style: Text.Raised
            styleColor: "#00000088"
        }

        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 12
            color: TetrisConfig.textDim
            text: "TETRIS"
        }
    }

    // ============================================================
    //  三栏主布局 / Main three-column layout
    // ============================================================
    Row {
        anchors.top: parent.top
        anchors.topMargin: 78
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: TetrisConfig.gap

        // ---------- 左栏：预览 + 操作说明 / Left panel ----------
        Rectangle {
            width: TetrisConfig.sidePanelW
            height: TetrisConfig.boardPxH
            color: TetrisConfig.panelBg
            radius: 10
            border { color: TetrisConfig.panelBorder; width: 1 }

            Column {
                anchors.fill: parent
                anchors.margins: 14
                spacing: 8

                Text {
                    font.pixelSize: 11
                    color: TetrisConfig.textDim
                    text: qsTr("")
                }

                NextPiecePreview {}

                Item { height: 6; width: 1 }   // 间距 / spacer

                Text {
                    font.pixelSize: 11
                    color: TetrisConfig.textDim
                    text: qsTr("操作说明")
                }

                Column {
                    spacing: 3
                    Row { spacing: 4
                        KeyLabel { text: "↓ / J" }
                        Text { font.pixelSize: 12; color: TetrisConfig.textBody; text: qsTr("下移") }
                    }
                    Row { spacing: 4
                        KeyLabel { text: "← / H" }
                        Text { font.pixelSize: 12; color: TetrisConfig.textBody; text: qsTr("左移") }
                    }
                    Row { spacing: 4
                        KeyLabel { text: "→ / L" }
                        Text { font.pixelSize: 12; color: TetrisConfig.textBody; text: qsTr("右移") }
                    }
                    Row { spacing: 4
                        KeyLabel { text: "空格" }
                        Text { font.pixelSize: 12; color: TetrisConfig.textBody; text: qsTr("旋转") }
                    }
                    Row { spacing: 4
                        KeyLabel { text: "Enter" }
                        Text { font.pixelSize: 12; color: TetrisConfig.textBody; text: qsTr("速降") }
                    }
                    Row { spacing: 4
                        KeyLabel { text: "P" }
                        Text { font.pixelSize: 12; color: TetrisConfig.textBody; text: qsTr("暂停") }
                    }
                }
            }
        }

        // ---------- 中栏：游戏棋盘 / Center: Game board ----------
        Rectangle {
            width:  TetrisConfig.boardPxW + 4
            height: TetrisConfig.boardPxH + 4
            color:  Qt.rgba(1, 1, 1, 0.08)
            radius: 8
            border { color: Qt.rgba(1, 1, 1, 0.06); width: 1 }

            Item {
                x: 2; y: 2
                width: TetrisConfig.boardPxW
                height: TetrisConfig.boardPxH

                // 游戏棋盘组件 / Game board component
                GameBoard { id: boardCanvas }

                // 消行动画闪烁 / Line-clear flash
                Rectangle {
                    id: lineFlash
                    anchors.fill: parent
                    color: Qt.rgba(1, 1, 1, 0.15)
                    radius: 4
                    opacity: 0
                    visible: opacity > 0

                    Connections {
                        target: GameEngine
                        function onLinesCleared(count) {
                            lineFlash.opacity = 1;
                            flashTimer.restart();
                        }
                    }

                    Timer {
                        id: flashTimer
                        interval: 100
                        onTriggered: lineFlash.opacity = 0
                    }
                }

                // 游戏结束覆盖层 / Game-over overlay
                Rectangle {
                    id: gameOverlay
                    anchors.fill: parent
                    color: Qt.rgba(0, 0, 0, 0.75)
                    radius: 4
                    opacity: 0
                    visible: opacity > 0

                    NumberAnimation {
                        id: overlayFadeIn
                        target: gameOverlay
                        property: "opacity"
                        from: 0; to: 1; duration: 400
                    }

                    Column {
                        anchors.centerIn: parent
                        spacing: 10

                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 28; font.weight: Font.Bold
                            color: TetrisConfig.textBright
                            text: qsTr("游戏结束")
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 48; font.weight: Font.ExtraBold
                            color: TetrisConfig.accent
                            text: GameEngine.score.toLocaleString()
                        }
                        Text {
                            anchors.horizontalCenter: parent.horizontalCenter
                            font.pixelSize: 14
                            color: TetrisConfig.textBody
                            text: qsTr("最终得分")
                        }
                        GameButton {
                            anchors.horizontalCenter: parent.horizontalCenter
                            label: qsTr("重新开始")
                            primary: true
                            onClicked: {
                                AudioEngine.startMusic();
                                GameEngine.startGame();
                                gameOverlay.opacity = 0;
                            }
                        }
                    }
                }

                // 暂停提示 / Pause indicator
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 36; font.weight: Font.Bold
                    color: Qt.rgba(1, 1, 1, 0.5)
                    text: qsTr("⏸ 暂停")
                    visible: GameEngine.paused
                }
            }
        }

        // ---------- 右栏：分数 + 按钮 / Right panel ----------
        ScorePanel { gameOverlayRef: gameOverlay }
    }

    // 游戏状态 → 覆盖层同步 / Game state → overlay sync
    Connections {
        target: GameEngine
        function onStateChanged() {
            if (GameEngine.gameOver) {
                overlayFadeIn.start();
            } else {
                gameOverlay.opacity = 0;
            }
        }
    }
}
