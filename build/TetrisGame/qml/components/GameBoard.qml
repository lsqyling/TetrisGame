import QtQuick
import GameBackend 1.0

// ============================================================
//  GameBoard.qml — 俄罗斯方块棋盘 / Game board
//
//  纯 UI 层：400 个 Rectangle 仅调用 C++ 高层渲染方法。
//  所有游戏逻辑（碰撞、颜色、幽灵位置）均在 C++ 中。
//
//  Pure UI layer: 400 Rectangles call high-level C++ methods.
//  All game logic (collision, colour, ghost) lives in C++.
//
//  层 / Layers:
//    1. 背景 + 网格 / Background + grid
//    2. 棋盘内容 / Board contents (200 cells, one C++ call each)
//    3. 幽灵叠加 / Ghost overlay (200 cells, one C++ call each)
// ============================================================
Item {
    id: root
    width: 300
    height: 600

    // === 背景 / Background ===
    Rectangle {
        anchors.fill: parent
        color: "#0d0d1a"
    }

    // === 网格线 / Grid lines ===
    Repeater {
        model: 21
        Rectangle {
            x: 0; y: index * 30
            width: 300; height: 1
            color: Qt.rgba(1, 1, 1, 0.03)
        }
    }
    Repeater {
        model: 11
        Rectangle {
            x: index * 30; y: 0
            width: 1; height: 600
            color: Qt.rgba(1, 1, 1, 0.03)
        }
    }

    // === 棋盘内容（锁定方块 + 当前方块）/ Board contents ===
    Repeater {
        model: 200

        Rectangle {
            property int row: Math.floor(index / 10)
            property int col: index % 10

            x: col * 30 + 1
            y: row * 30 + 1
            width: 28
            height: 28

            // 颜色由 C++ 全权决定 / Colour determined entirely by C++
            color: {
                GameEngine.stateRevision;
                return GameEngine.cellColor(row, col);
            }

            // 3D 高光/阴影 / 3D highlight/shadow
            Rectangle {
                anchors.left: parent.left; anchors.top: parent.top
                width: parent.width; height: 3
                color: Qt.rgba(1, 1, 1, 0.2)
                visible: parent.color !== "transparent"
            }
            Rectangle {
                anchors.left: parent.left; anchors.top: parent.top
                width: 3; height: parent.height
                color: Qt.rgba(1, 1, 1, 0.2)
                visible: parent.color !== "transparent"
            }
            Rectangle {
                anchors.right: parent.right; anchors.bottom: parent.bottom
                width: parent.width; height: 3
                color: Qt.rgba(0, 0, 0, 0.3)
                visible: parent.color !== "transparent"
            }
            Rectangle {
                anchors.right: parent.right; anchors.bottom: parent.bottom
                width: 3; height: parent.height
                color: Qt.rgba(0, 0, 0, 0.3)
                visible: parent.color !== "transparent"
            }
        }
    }

    // === 幽灵方块叠加层 / Ghost piece overlay (20% opacity) ===
    Repeater {
        model: 200

        Rectangle {
            property int row: Math.floor(index / 10)
            property int col: index % 10

            x: col * 30 + 1
            y: row * 30 + 1
            width: 28; height: 28
            opacity: 0.2
            z: 1

            // 可见性和颜色均由 C++ 决定 / Visibility & colour from C++
            visible: {
                GameEngine.stateRevision;
                return GameEngine.ghostVisible(row, col);
            }

            color: {
                GameEngine.stateRevision;
                return GameEngine.ghostColor();
            }
        }
    }
}
