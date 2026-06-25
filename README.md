# 俄罗斯方块 — Classic Tetris

Qt 6.8 / QML + C++20 经典俄罗斯方块，无边框窗口、程序化芯片音效、Korobeiniki 背景音乐。

<p align="center">
  <img src="https://img.shields.io/badge/Qt-6.8-green" alt="Qt 6.8">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue" alt="C++20">
  <img src="https://img.shields.io/badge/CMake-3.25%2B-red" alt="CMake 3.25+">
</p>

---

## 目录

- [快速开始](#快速开始)
- [项目架构](#项目架构)
- [核心设计](#核心设计)
  - [C++ ↔ QML 桥接](#c--qml-桥接)
  - [游戏引擎](#游戏引擎-gameengine)
  - [棋盘渲染](#棋盘渲染-gameboardqml)
  - [音频引擎](#音频引擎-audioengine)
  - [窗口 UI](#窗口-ui-mainqml)
- [游戏控制](#游戏控制)
- [游戏机制](#游戏机制)
- [双实现模式](#双实现模式)
- [关键设计决策](#关键设计决策)

---

## 快速开始

```powershell
# 配置（指向你的 Qt 6.8 安装路径）
cmake -B build -DCMAKE_PREFIX_PATH=D:/dev/tools/Qt/6.8.3/msvc2022_64

# 编译
cmake --build build --config Release

# 运行
./build/Release/TetrisGame.exe
```

**依赖：** Qt 6.8+ (Quick + Multimedia)、CMake 3.25+、MSVC 2022 (C++20)

---

## 项目架构

```
TetrisGame/
├── CMakeLists.txt                     # 构建配置 (qt_add_qml_module × 2)
├── main.cpp                           # 入口：注册 C++ 单例、加载 QML 模块
├── cmake/                             # CMake 辅助 (编译选项、windeployqt)
├── src/
│   ├── GameEngine.h / .cpp            # C++ 游戏引擎
│   └── AudioEngine.h / .cpp           # C++ 芯片音效合成
├── qml/
│   ├── Main.qml                       # 根窗口 (无边框、浮动按钮、拖拽)
│   ├── qmldir                         # QML 模块清单
│   ├── basic/
│   │   └── TetrisConfig.qml           # 单例配置 (颜色、布局常量)
│   ├── components/
│   │   ├── GameBoard.qml              # 棋盘 (原生 Rectangle + Repeater)
│   │   ├── NextPiecePreview.qml       # 下一方块预览 (Canvas)
│   │   ├── ScorePanel.qml             # 分数/等级/消行 + 开始/暂停按钮
│   │   ├── GameButton.qml             # 主题按钮 (primary/secondary)
│   │   ├── KeyLabel.qml               # 键盘快捷键徽章
│   │   └── BackgroundEffect.qml       # 粒子动画背景 (Canvas)
│   └── pages/
│       └── GamePage.qml               # 三栏主布局 (左/中/右)
└── index.html                         # 浏览器参考实现 (无编译，逻辑相同)
```

### 分层职责

| 层 | 技术 | 职责 |
|----|------|------|
| **游戏逻辑** | C++ `GameEngine` | 棋盘状态、碰撞检测、消行计分、7-bag 随机、墙踢 |
| **音频** | C++ `AudioEngine` | PCM 波形合成、BGM 循环、SFX 播放 |
| **渲染** | QML `Rectangle` + `Repeater` | 棋盘 400 格原生渲染、方块颜色绑定 |
| **UI** | QML Window | 无边框窗口、浮动按钮、全窗口拖拽、键盘处理 |

---

## 核心设计

### C++ ↔ QML 桥接

C++ 对象通过 `qmlRegisterSingletonInstance` 注册为 QML 单例——**不使用 `QQmlContext::setContextProperty()`**（后者在 Qt 6 中不会传递到 QML 模块）。

```cpp
// main.cpp
GameEngine  gameEngine;
AudioEngine audioEngine;
qmlRegisterSingletonInstance("GameBackend", 1, 0, "GameEngine",  &gameEngine);
qmlRegisterSingletonInstance("GameBackend", 1, 0, "AudioEngine", &audioEngine);
```

```qml
// QML 中直接使用
import GameBackend 1.0
Text { text: GameEngine.score.toLocaleString() }
GameButton { onClicked: GameEngine.startGame() }
```

### QML 模块系统

项目包含两个 QML 模块：

| 模块 | URI | 类型 | 说明 |
|------|-----|------|------|
| TetrisGame | `TetrisGame` | `qt_add_qml_module` | 主模块，包含所有页面和组件 |
| TetrisConfig | `TetrisConfig` | 静态库 + `QT_QML_SINGLETON_TYPE` | 全局配置单例（颜色、布局） |

`qmldir` 显式列出所有组件名称和路径，确保 qmlcachegen 正确解析模块内的类型引用。

### 游戏引擎 (`GameEngine`)

**状态模型：**

```
m_board: QList<QList<int>>    → 20 行 × 10 列
                                 0 = 空
                                 1…7 = 方块类型 + 1（1-based 存储）

m_currentPiece: Piece         → { type, shape, x, y }
                                 当前下落中的方块

m_bag: QList<int>             → 7-bag 随机队列
```

**核心算法：**

| 算法 | 细节 |
|------|------|
| 随机 | 7-bag Fisher-Yates 洗牌，袋空自动填充 |
| 碰撞 | 遍历 shape 矩阵 + 边界检查 (bx<0、bx>=COLS、by>=ROWS) |
| 旋转 | 顺时针矩阵转置 + 8 位墙踢偏移 `{(0,0),(-1,0),(1,0),(0,-1),(-1,-1),(1,-1),(-2,0),(2,0)}` |
| 幽灵 | `computeGhostY()` 逐格下落至碰撞，渲染时 20% 透明度 |
| 重力 | `QTimer` 单次触发模式，15 级速度 (800ms→40ms) |
| 计分 | `{0,100,300,500,800}[lines] × level`，软降 +1/格，硬降 +2/格 |
| 消行 | 从底部扫描→移除满行→顶部补空行→emit `linesCleared(count)` |

**单格访问器（关键设计决策）：**

Qt 6.8 qmlcachegen 无法在 QML JavaScript 中可靠访问嵌套 `QVariantList`（`board[r][c]`、`shape[r][c]` 静默失败）。解决方案是提供返回单个 `int` 的 `Q_INVOKABLE` 方法：

```cpp
Q_INVOKABLE int boardCell(int row, int col) const;        // 锁定方块 (0=空, 1-7)
Q_INVOKABLE int currentPieceCell(int row, int col) const;  // 激活方块 (0/1)
Q_INVOKABLE int ghostCell(int row, int col) const;         // 幽灵方块 (0/1)
```

**状态修订计数器：**

QML 绑定系统只能追踪 `Q_PROPERTY` 依赖，无法追踪 `Q_INVOKABLE` 方法的内部依赖。因此在 `GameEngine` 中增加了一个计数器属性：

```cpp
Q_PROPERTY(int stateRevision READ stateRevision NOTIFY stateChanged)
// notifyAll() 中：m_stateRevision++; emit stateChanged();
```

在 QML 中，每个绑定第一行读取 `GameEngine.stateRevision`，确保 `stateChanged` 时强制执行重算。

### 棋盘渲染 (`GameBoard.qml`)

**核心决策：零 Canvas，纯原生 QML `Rectangle`。**

Qt 6.8 的 Canvas `onPaint` 在此项目配置下完全无法使用——内联定义、独立组件文件、`requestPaint()` 持续循环、Timer 触发、信号连接……全部失败。这是 qmlcachegen 编译器层面的问题。

**最终方案：400 个 `Rectangle`，两层 `Repeater`。**

| 层 | Repeater | z | 内容 |
|----|----------|---|------|
| 主体 | 200 项 (20×10) | 0 | 锁定方块 + 激活方块（合并绑定） |
| 幽灵 | 200 项 (20×10) | 1 | 幽灵方块 (opacity: 0.2) |

```qml
// 每格仅调用一个 C++ 高层方法——零 QML 条件逻辑
// Each cell calls ONE high-level C++ method — zero QML conditionals
color: {
    GameEngine.stateRevision;
    return GameEngine.cellColor(row, col);
}
```

所有游戏逻辑（当前方块优先级、游戏结束检查、锁定方块颜色映射）封装在 `GameEngine::cellColor()` 中。幽灵层同理使用 `ghostVisible()` + `ghostColor()`。每个方块带有 4 个子 `Rectangle`（高光 + 阴影），实现 3D 立体效果。

### 音频引擎 (`AudioEngine`)

**程序化 PCM 合成——无外部音频文件。**

| 组件 | 技术 |
|------|------|
| 格式 | 44.1kHz, 16-bit, 单声道 |
| 音效波形 | 方波 + 正弦混合，快速包络衰减 |
| BGM 波形 | 旋律：方波；低音：三角波 |
| 播放 | `QAudioSink` + `QBuffer` |
| 连接管理 | `Qt::SingleShotConnection` 防止累积 |

**BGM 生命周期管理：**

```
startMusic()     → m_musicPlaying=true, startBGMPlayback()
pauseMusic()     → m_audioSink->suspend()    (保留播放位置)
resumeMusic()    → m_audioSink->resume()     (断点续播)
playBuffer(sfx)  → m_audioSink->stop()       (打断 BGM)
                 → 播放 SFX
                 → QTimer::singleShot(50ms)  (SFX 结束后延迟恢复 BGM)
stopMusic()      → m_musicPlaying=false, m_audioSink->stop()
```

暂停/继续使用 `suspend()`/`resume()` 而非 `stop()`/`start()`，BGM 从断点续播不间断。SFX 中断后通过 50ms 延迟定时器恢复——快速连续音效时只执行最后一次恢复。

### 窗口 UI (`Main.qml`)

**无边框窗口 + 浮动按钮 + 全窗口拖拽。**

```
z 层级 (顶 → 底)：
  z:10  → 关闭/最小化浮动按钮  (hover 150ms 淡入)
  z:1   → GamePage             (开始/暂停按钮、棋盘)
  z:0   → 全窗口拖拽 MouseArea  (空白区域 startSystemMove)
  z:0   → BackgroundEffect     (粒子动画背景)
```

| 特性 | 实现 |
|------|------|
| 无边框 | `flags: Qt.FramelessWindowHint \| Qt.Window` |
| 关闭按钮 | 右上角，橘红 (`#e81123`)，hover 淡入，leave 淡出 |
| 最小化按钮 | 关闭按钮左侧，半透明，hover 淡入，leave 淡出 |
| 窗口拖拽 | 全窗口 MouseArea (z:0)，空白区域 `startSystemMove()` |
| 键盘焦点 | `Component.onCompleted: contentItem.forceActiveFocus()` |

---

## 游戏控制

| 按键 | 操作 |
|------|------|
| `↓` / `J` | 下移 (软降 +1 分/行) |
| `←` / `H` | 左移 |
| `→` / `L` | 右移 |
| `Space` | 顺时针旋转 (+ 8 位墙踢) |
| `Enter` | 硬降 (+2 分/行) / 开始游戏 |
| `P` | 暂停 / 继续 |
| `M` | 切换背景音乐 |

---

## 游戏机制

### 方块类型

| 索引 | 类型 | 尺寸 | 颜色 |
|------|------|------|------|
| 0 | I | 4×4 | `#00f0f0` 青色 |
| 1 | O | 2×2 | `#f0f000` 黄色 |
| 2 | T | 3×3 | `#a000f0` 紫色 |
| 3 | S | 3×3 | `#00f000` 绿色 |
| 4 | Z | 3×3 | `#f00000` 红色 |
| 5 | J | 3×3 | `#0000f0` 蓝色 |
| 6 | L | 3×3 | `#f0a000` 橙色 |

I 方块从 y=-1 生成（第 0 行以上）。

### 速度曲线

| 等级 | 间隔 | 等级 | 间隔 |
|------|------|------|------|
| 1 | 800ms | 6 | 380ms |
| 2 | 720ms | 7 | 290ms |
| 3 | 630ms | 8 | 200ms |
| 4 | 550ms | 9 | 140ms |
| 5 | 470ms | 10-15 | 100→40ms |

每消除 10 行等级 +1，最高 15 级。

### 计分

| 消行数 | 基础分 |
|--------|--------|
| 1 | 100 |
| 2 | 300 |
| 3 | 500 |
| 4 (Tetris) | 800 |

最终得分 = 基础分 × 当前等级。软降每格 +1，硬降每格 +2。

---

## 双实现模式

项目包含两个功能相同的实现：

| 实现 | 入口 | 渲染 | 音频 |
|------|------|------|------|
| Qt / C++ | `main.cpp` → QML | Rectangle + Repeater | C++ `QAudioSink` |
| 浏览器 | `index.html` | HTML Canvas 2D | Web Audio API |

两者共享**完全相同的游戏逻辑**——方块形状、墙踢、计分公式、7-bag 随机算法、速度等级、幽灵方块计算。HTML 版本无需编译，可直接打开运行，适合逻辑原型设计。

---

## 关键设计决策

### Qt 6.8 qmlcachegen 限制及应对

| 问题 | 影响范围 | 解决方案 |
|------|----------|----------|
| Canvas `onPaint` 不触发 | 游戏棋盘不可见 | 原生 `Rectangle` + `Repeater` 替代 Canvas |
| 嵌套 `QVariantList` 访问失败 | `board[r][c]` 返回错误值 | 逐格 `Q_INVOKABLE int` 访问器 |
| QML 绑定不追踪 `Q_INVOKABLE` 依赖 | 状态变化不重绘 | `stateRevision` Q_PROPERTY 强制重算 |
| `;` 作为属性分隔符 | 编译错误 | 所有属性独立成行 (QTP0004 已禁用) |

### 性能考量

- **400 个 Rectangle** (200 主层 + 200 幽灵层)：每个 `stateChanged` 周期内 400 次绑定重算 + ~600 次 C++ 方法调用。在 30fps 下约每秒 12,000 次调用——桌面平台完全可忽略不计。
- 原生 QML Rectangle 的 GPU 合成比 Canvas 2D 软件光栅化性能更优。
- 单格访问器（`boardCell` 等）只进行基本边界检查 + 数组索引，开销极小。

---

## 重构历程 / Refactoring Journey

### 第一阶段：Canvas → Rectangle

原始设计使用 QML `Canvas` Context2D 渲染棋盘。Qt 6.8 qmlcachegen 存在编译器级缺陷：`onPaint` 完全不触发（无论 `requestPaint()` 自循环、Timer 还是信号连接均失败）。

**方案：** 用 400 个原生 QML `Rectangle`（`Repeater` 两层：主层 200 + 幽灵层 200）替代 Canvas。QML 绑定系统自动响应 C++ `stateChanged` 信号，无需手动触发重绘。

### 第二阶段：QVariantList → 逐格 int 访问器

Canvas 方案失败后，QML 通过 `QVariantList` 属性（`boardData`、`currentPieceShape`）读取游戏状态。qmlcachegen 编译的 QML JavaScript 无法可靠访问嵌套 `QVariantList`（`board[r][c]`、`shape[r][c]` 静默返回错误值）。

**方案：** 新增 `Q_INVOKABLE int boardCell(row, col)` 等逐格方法，返回单个 `int`。新增 `stateRevision` 计数器 `Q_PROPERTY`——所有绑定首行读取它以强制重算（因 QML 绑定系统无法追踪 `Q_INVOKABLE` 方法的内部依赖）。

### 第三阶段：QML JS 现代化

将全部 QML JavaScript 代码中的 `var` 替换为 `const`/`let`：

| 文件 | 变更 | 说明 |
|------|------|------|
| `NextPiecePreview.qml` | 12 处 | `onPaint` 循环 + `drawBlock` 函数 |
| `BackgroundEffect.qml` | 20+ 处 | 粒子初始化 + `onPaint` 双循环渲染 |
| `GameBoard.qml` | 1 处 | `boardCell()` 返回值 |

### 第四阶段：逻辑下沉 C++

QML 绑定中包含游戏条件判断（当前方块优先级、游戏结束状态、幽灵位置计算）和重复 C++ 调用（`pieceColor()` 每帧被调用 ~400 次）。

**方案：** 新增三个 C++ 高层渲染方法，封装全部游戏逻辑：

```cpp
// 一个调用返回最终颜色（处理当前方块 > 锁定方块优先级）
Q_INVOKABLE QString cellColor(int row, int col) const;

// 一个调用返回幽灵可见性（处理幽灵位置 + 游戏结束状态）
Q_INVOKABLE bool ghostVisible(int row, int col) const;

// 一个调用返回幽灵颜色（缓存，全层复用）
Q_INVOKABLE QString ghostColor() const;
```

**Before → After（GameBoard.qml 主层）：**

```
// 重构前：6 行 QML JS，3 种 C++ 方法，双层条件判断
color: {
    GameEngine.stateRevision;
    if (GameEngine.currentPieceCell(row, col) !== 0 && !GameEngine.gameOver)
        return GameEngine.pieceColor(GameEngine.currentPieceType);
    const bv = GameEngine.boardCell(row, col);
    if (bv !== 0) return GameEngine.pieceColor(bv - 1);
    return "transparent";
}

// 重构后：1 个 C++ 调用，零 QML 条件逻辑
color: {
    GameEngine.stateRevision;
    return GameEngine.cellColor(row, col);
}
```

### 最终架构原则

```
┌─────────────────────────────────────────┐
│  C++ GameEngine                         │
│  · 全部游戏状态和逻辑                      │
│  · 高层渲染方法 (cellColor 等)             │
│  · stateRevision 变更通知                 │
└──────────┬──────────────────────────────┘
           │ Q_PROPERTY / Q_INVOKABLE
           ▼
┌─────────────────────────────────────────┐
│  QML GameBoard                          │
│  · 400 个 Rectangle (Repeater)           │
│  · 每个绑定 = 1 行 C++ 调用 + stateRevision│
│  · 零游戏逻辑、零条件判断                   │
└─────────────────────────────────────────┘
```

---

## 许可

MIT License — Copyright (c) 2026
