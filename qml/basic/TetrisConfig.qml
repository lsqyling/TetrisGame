pragma Singleton

import QtQuick

// ============================================================
//  TetrisConfig.qml — 全局配置单例 / Global config singleton
//  主题颜色 + 布局常量 / Theme colours + layout constants
// ============================================================
QtObject {
    // --- 主题颜色 / Theme colours ---
    readonly property color bgColor:        "#0f0f23"
    readonly property color panelBg:        Qt.rgba(1, 1, 1, 0.04)
    readonly property color panelBorder:    Qt.rgba(1, 1, 1, 0.06)
    readonly property color textDim:        "#667"
    readonly property color textBody:       "#889"
    readonly property color textBright:     "#eee"
    readonly property color accent:         "#ffd93d"
    readonly property color primaryBtn:     "#ee5a24"
    readonly property color primaryHover:   "#ff6348"
    readonly property color titleColor:     "#ff6b6b"

    // --- 布局 / Layout ---
    readonly property int blockSize:    30
    readonly property int cols:         10
    readonly property int rows:         20
    readonly property int previewSize:  24
    readonly property int sidePanelW:   160
    readonly property int gap:          16
    readonly property int boardPxW:     cols * blockSize    // 300
    readonly property int boardPxH:     rows * blockSize    // 600

    // --- 窗口尺寸（不可调整）/ Window dimensions (non-resizable) ---
    readonly property int windowW:      sidePanelW + gap + boardPxW + gap + sidePanelW + 48
    readonly property int windowH:      boardPxH + 120
}
