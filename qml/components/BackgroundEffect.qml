import QtQuick
import GameBackend 1.0

// ============================================================
//  BackgroundEffect.qml — 粒子动画背景 / Animated particle bg
//  星空粒子 + 浮游方块 + 渐变色彩 / Stars + floating pieces + gradient
//  使用 requestPaint() 持续循环渲染 / Continuous rendering loop
// ============================================================
Canvas {
    id: bgCanvas

    property var stars: []
    property var floaters: []     // { type, x, y, speed, alpha, size, rotation }
    property int frameCount: 0

    // 初始化粒子系统 / Initialise particle systems
    Component.onCompleted: {
        const w = width, h = height;
        for (let i = 0; i < 80; i++) {
            stars.push({
                x: Math.random() * w,
                y: Math.random() * h,
                size: 0.5 + Math.random() * 1.5,
                alpha: 0.15 + Math.random() * 0.35,
                speed: 0.02 + Math.random() * 0.06
            });
        }

        for (let pi = 0; pi < 6; pi++) {
            floaters.push({
                type: Math.floor(Math.random() * 7),
                x: Math.random() * w,
                y: Math.random() * h,
                speed: 0.15 + Math.random() * 0.3,
                alpha: 0.03 + Math.random() * 0.05,
                size: 12 + Math.random() * 20,
                rotation: Math.random() * 360,
                rotSpeed: -0.3 + Math.random() * 0.6
            });
        }
        bgCanvas.requestPaint();
    }

    onPaint: {
        const ctx = getContext("2d");
        const w = width, h = height;
        frameCount++;

        // 动态渐变背景 / Animated background gradient
        const t = frameCount * 0.002;
        const grad = ctx.createLinearGradient(0, 0, w, h);
        grad.addColorStop(0, Qt.rgba(
            0.05 + 0.03 * Math.sin(t),
            0.05 + 0.03 * Math.cos(t * 0.7),
            0.12 + 0.04 * Math.sin(t * 0.5), 1));
        grad.addColorStop(0.5, Qt.rgba(0.06, 0.06, 0.12, 1));
        grad.addColorStop(1, Qt.rgba(
            0.08 + 0.03 * Math.cos(t * 0.8 + 1),
            0.08 + 0.03 * Math.sin(t * 0.6 + 2),
            0.14 + 0.04 * Math.cos(t * 0.4 + 3), 1));
        ctx.fillStyle = grad;
        ctx.fillRect(0, 0, w, h);

        // 星空粒子 / Star field
        for (let i = 0; i < stars.length; i++) {
            const s = stars[i];
            const twinkle = 0.7 + 0.3 * Math.sin(frameCount * 0.02 + i);
            ctx.globalAlpha = s.alpha * twinkle;
            ctx.fillStyle = "#ffffff";
            ctx.fillRect(s.x, s.y, s.size, s.size);

            s.y -= s.speed;
            if (s.y < -5) { s.y = h + 5; s.x = Math.random() * w; }
        }

        // 浮游方块 / Floating tetromino shapes
        ctx.globalAlpha = 1.0;
        for (let j = 0; j < floaters.length; j++) {
            const f = floaters[j];
            ctx.save();
            ctx.translate(f.x, f.y);
            ctx.rotate(f.rotation * Math.PI / 180);
            ctx.globalAlpha = f.alpha;

            const shape = GameEngine.pieceShape(f.type);
            if (!shape) { ctx.restore(); continue; }

            const bs = f.size;
            const rows = shape.length, cols = shape[0].length;
            ctx.fillStyle = GameEngine.pieceColor(f.type);
            for (let r = 0; r < rows; r++)
                for (let c = 0; c < cols; c++)
                    if (shape[r][c])
                        ctx.fillRect(c * bs, r * bs, bs - 1, bs - 1);

            ctx.restore();

            f.y -= f.speed;
            f.rotation += f.rotSpeed;
            if (f.y < -100) {
                f.y = h + 50; f.x = Math.random() * w;
                f.type = Math.floor(Math.random() * 7);
                f.alpha = 0.03 + Math.random() * 0.05;
                f.size = 12 + Math.random() * 20;
            }
        }

        // 下一帧 / Schedule next frame
        bgCanvas.requestPaint();
    }
}
