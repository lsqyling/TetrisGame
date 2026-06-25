#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QObject>
#include <QTimer>
#include <QVariantList>
#include <QStringList>
#include <QList>
#include <QPoint>

// ============================================================
//  GameEngine — 俄罗斯方块游戏逻辑核心 / Core game logic
//
//  负责：棋盘状态、碰撞检测、7-bag 随机、墙踢旋转、
//        消行计分、重力定时器
//  Responsibilities: board state, collision, 7-bag random,
//        wall kicks, line clearing, scoring, gravity timer
// ============================================================
class GameEngine : public QObject
{
    Q_OBJECT

    // --- QML 可读属性 / Read-only properties consumed by QML ---
    Q_PROPERTY(int          score       READ score       NOTIFY stateChanged)
    Q_PROPERTY(int          level       READ level       NOTIFY stateChanged)
    Q_PROPERTY(int          lines       READ lines       NOTIFY stateChanged)
    Q_PROPERTY(bool         gameRunning READ gameRunning NOTIFY stateChanged)
    Q_PROPERTY(bool         gameOver    READ gameOver    NOTIFY stateChanged)
    Q_PROPERTY(bool         paused      READ paused      NOTIFY stateChanged)
    Q_PROPERTY(QVariantList boardData   READ boardData   NOTIFY stateChanged)
    Q_PROPERTY(int          nextPieceType READ nextPieceType NOTIFY stateChanged)
    Q_PROPERTY(int          currentPieceType READ currentPieceType NOTIFY stateChanged)
    Q_PROPERTY(int          currentPieceX   READ currentPieceX   NOTIFY stateChanged)
    Q_PROPERTY(int          currentPieceY   READ currentPieceY   NOTIFY stateChanged)
    Q_PROPERTY(int          ghostY          READ ghostY          NOTIFY stateChanged)
    Q_PROPERTY(QVariantList currentPieceShape READ currentPieceShape NOTIFY stateChanged)
    // 状态修订计数器——QML 绑定读取它以强制在每次状态变更时重算
    // Revision counter — read by QML bindings to force re-evaluation
    Q_PROPERTY(int          stateRevision READ stateRevision NOTIFY stateChanged)

    // --- 常量 / Constants (never change) ---
    Q_PROPERTY(int cols READ cols CONSTANT)
    Q_PROPERTY(int rows READ rows CONSTANT)
    Q_PROPERTY(int blockSize READ blockSize CONSTANT)
    Q_PROPERTY(QStringList pieceColors READ pieceColors CONSTANT)

public:
    explicit GameEngine(QObject *parent = nullptr);
    ~GameEngine() override = default;

    // ---- 属性 getter / Property getters ----
    int          score()            const { return m_score; }
    int          level()            const { return m_level; }
    int          lines()            const { return m_lines; }
    bool         gameRunning()      const { return m_gameRunning; }
    bool         gameOver()         const { return m_gameOver; }
    bool         paused()           const { return m_paused; }
    QVariantList boardData()        const;
    int          nextPieceType()    const { return m_nextPieceType; }
    int          currentPieceType() const;
    int          currentPieceX()    const;
    int          currentPieceY()    const;
    int          ghostY()           const;
    QVariantList currentPieceShape() const;
    int          stateRevision()    const { return m_stateRevision; }

    // 常量 / Constants
    int          cols()       const { return COLS; }
    int          rows()       const { return ROWS; }
    int          blockSize()  const { return BLOCK_SIZE; }
    QStringList  pieceColors() const { return s_pieceColors; }

    // ---- QML 可调用方法 / Invokable (called from QML) ----
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void restartGame();
    Q_INVOKABLE void moveLeft();
    Q_INVOKABLE void moveRight();
    Q_INVOKABLE void moveDown();
    Q_INVOKABLE void rotatePiece();
    Q_INVOKABLE void hardDrop();
    Q_INVOKABLE void togglePause();

    // 单格渲染——所有游戏逻辑在 C++ 中完成，QML 仅读取最终颜色
    // Per-cell rendering — all game logic in C++, QML just reads final colour
    Q_INVOKABLE QString cellColor(int row, int col) const;
    Q_INVOKABLE bool    ghostVisible(int row, int col) const;
    Q_INVOKABLE QString ghostColor() const;

    // 底层工具（供其他 QML 组件使用）/ Low-level utilities (for other QML)
    Q_INVOKABLE QString pieceColor(int type) const;
    Q_INVOKABLE QVariantList pieceShape(int type) const;
    Q_INVOKABLE int boardCell(int row, int col) const;
    Q_INVOKABLE int currentPieceCell(int row, int col) const;
    Q_INVOKABLE int ghostCell(int row, int col) const;

signals:
    // 任何游戏状态属性变化时发出 / Emitted on any state change
    void stateChanged();
    // 消行时发出（用于闪烁动画）/ Emitted when lines cleared (flash anim)
    void linesCleared(int count);

private slots:
    void tick();

private:
    // ---- 常量 / Constants ----
    static constexpr int COLS       = 10;
    static constexpr int ROWS       = 20;
    static constexpr int BLOCK_SIZE = 30;

    // 方块颜色（按类型索引 0-6）/ Piece colours indexable by type
    static const QStringList s_pieceColors;
    // 等级速度（重力间隔 ms）/ Level speeds (gravity interval ms)
    static const QList<int>  s_levelSpeeds;
    // 方块形状 [类型][行][列] (0/1) / Piece shapes: [type][row][col]
    static const QList<QList<QList<int>>> s_pieceShapes;

    // ---- 内部状态 / Internal state ----
    QList<QList<int>> m_board;       // [行][列] 0=空, 1-7=类型+1
    int               m_score   = 0;
    int               m_level   = 1;
    int               m_lines   = 0;
    int               m_stateRevision = 0;
    bool              m_gameRunning = false;
    bool              m_gameOver    = false;
    bool              m_paused      = false;

    // 当前下落中的方块 / Current falling piece
    struct Piece {
        int               type{-1};  // 0-6
        QList<QList<int>> shape;       // 2-D 0/1 矩阵
        int               x{0},  y{0};
    };
    Piece             m_currentPiece;
    int               m_nextPieceType = 0;

    // 7-bag 随机队列 / 7-bag randomiser
    QList<int>        m_bag;

    // 重力定时器 / Gravity timer
    QTimer           *m_timer = nullptr;

    // ---- 辅助方法 / Helpers ----
    void          initBoard();
    void          shuffleBag();
    int           drawType();
    void          spawnPiece();
    bool          checkCollision(const QList<QList<int>> &shape,
                                 int x, int y) const;
    void          lockPiece();
    void          clearLines();
    int           computeGhostY() const;
    void          startTimer();
    void          stopTimer();
    void          updateTimer();
    void          notifyAll();        // emit stateChanged
};

#endif // GAMEENGINE_H
