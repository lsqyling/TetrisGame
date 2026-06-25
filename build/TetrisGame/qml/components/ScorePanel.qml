import QtQuick
import TetrisConfig
import GameBackend 1.0

// ============================================================
//  ScorePanel.qml — 右侧信息面板 / Right side panel
//  显示分数、等级、消行数 + 开始/暂停按钮
//  Score, level, lines + Start/Pause buttons
// ============================================================
Rectangle {
    id: panel

    property var gameOverlayRef: null

    width: TetrisConfig.sidePanelW
    height: TetrisConfig.boardPxH
    color: TetrisConfig.panelBg
    radius: 10
    border { color: TetrisConfig.panelBorder; width: 1 }

    Column {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 16

        // 分数 / Score
        Column {
            spacing: 2
            Text {
                font.pixelSize: 11
                color: TetrisConfig.textDim
                text: qsTr("分数")
            }
            Text {
                font.pixelSize: 28; font.weight: Font.Bold
                color: TetrisConfig.accent
                text: GameEngine.score.toLocaleString()
            }
        }

        // 等级 / Level
        Column {
            spacing: 2
            Text {
                font.pixelSize: 11
                color: TetrisConfig.textDim
                text: qsTr("等级")
            }
            Text {
                font.pixelSize: 22; font.weight: Font.Bold
                color: TetrisConfig.textBright
                text: GameEngine.level
            }
        }

        // 消行 / Lines
        Column {
            spacing: 2
            Text {
                font.pixelSize: 11
                color: TetrisConfig.textDim
                text: qsTr("消行")
            }
            Text {
                font.pixelSize: 22; font.weight: Font.Bold
                color: TetrisConfig.textBright
                text: GameEngine.lines
            }
        }

        Item { height: 6; width: 1 }   // 间距 / spacer

        // 开始按钮 / Start button
        GameButton {
            width: parent.width
            label: GameEngine.gameRunning ? qsTr("游戏中") : qsTr("开始游戏")
            primary: true
            enabled: !GameEngine.gameRunning
            onClicked: {
                AudioEngine.startMusic()
                GameEngine.startGame()
                if (panel.gameOverlayRef)
                    panel.gameOverlayRef.opacity = 0
            }
        }

        // 暂停按钮 / Pause button
        GameButton {
            width: parent.width
            label: GameEngine.paused ? qsTr("继续 ▶") : qsTr("暂停")
            enabled: GameEngine.gameRunning && !GameEngine.gameOver
            onClicked: GameEngine.togglePause()
        }
    }
}
