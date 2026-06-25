import QtQuick
import GameBackend 1.0

// ============================================================
//  NextPiecePreview.qml — 下一方块预览 / Next piece preview
//  双重刷新：Timer + stateChanged 信号
//  Dual refresh: Timer + stateChanged signal
// ============================================================
Canvas {
    id: nextCanvas
    width: 120
    height: 100

    // 持续刷新定时器 / Continuous refresh timer
    Timer {
        id: previewTimer
        interval: 50
        repeat: true
        running: GameEngine.gameRunning && !GameEngine.paused
        onTriggered: nextCanvas.requestPaint()
    }

    onPaint: {
        const ctx = getContext("2d");
        if (!ctx) return;
        ctx.clearRect(0, 0, width, height);

        const pType = GameEngine.nextPieceType;
        if (pType < 0) return;

        const shape = GameEngine.pieceShape(pType);
        if (!shape || shape.length === 0) return;

        const shRows = shape.length;
        const shCols = shape[0].length;
        const ps = 24;
        const ox = (width  - shCols * ps) * 0.5;
        const oy = (height - shRows * ps) * 0.5;
        const color = GameEngine.pieceColor(pType);

        for (let r = 0; r < shRows; r++) {
            const sRow = shape[r];
            if (!sRow) continue;
            for (let c = 0; c < shCols; c++) {
                if (!sRow[c]) continue;
                nextCanvas.drawBlock(ctx, ox / ps + c, oy / ps + r, color, 1.0, ps);
            }
        }
    }

    // 绘制单个方块 / Draw a single block
    function drawBlock(ctx, x, y, color, alpha, bs) {
        ctx.save();
        ctx.globalAlpha = alpha;

        const cx = x * bs, cy = y * bs;

        // 主体 / Main body
        ctx.fillStyle = color;
        ctx.fillRect(cx + 1, cy + 1, bs - 2, bs - 2);

        // 左上高光 / Top-left highlight
        ctx.fillStyle = Qt.rgba(1, 1, 1, 0.25 * alpha);
        ctx.fillRect(cx + 1, cy + 1, bs - 2, 3);
        ctx.fillRect(cx + 1, cy + 1, 3, bs - 2);

        // 右下阴影 / Bottom-right shadow
        ctx.fillStyle = Qt.rgba(0, 0, 0, 0.3 * alpha);
        ctx.fillRect(cx + 1, cy + bs - 4, bs - 2, 3);
        ctx.fillRect(cx + bs - 4, cy + 1, 3, bs - 2);

        // 径向发光 / Radial glow
        const grad = ctx.createRadialGradient(
                    cx + bs/2, cy + bs/2, 1,
                    cx + bs/2, cy + bs/2, bs/2);
        grad.addColorStop(0, Qt.rgba(1, 1, 1, 0.15 * alpha));
        grad.addColorStop(1, Qt.rgba(1, 1, 1, 0));
        ctx.fillStyle = grad;
        ctx.fillRect(cx + 1, cy + 1, bs - 2, bs - 2);

        ctx.restore();
    }

    // 信号触发刷新 / Signal-triggered refresh
    Connections {
        target: GameEngine
        function onStateChanged() { nextCanvas.requestPaint() }
    }

    // 初始绘制 / Initial paint
    Component.onCompleted: nextCanvas.requestPaint()
}
