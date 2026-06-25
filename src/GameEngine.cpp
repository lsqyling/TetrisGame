#include "GameEngine.h"

#include <QRandomGenerator>
#include <QDateTime>

// ============================================================
//  静态数据 / Static data
// ============================================================

const QStringList GameEngine::s_pieceColors = {
    QStringLiteral("#00f0f0"),   // 0=I  青 cyan
    QStringLiteral("#f0f000"),   // 1=O  黄 yellow
    QStringLiteral("#a000f0"),   // 2=T  紫 purple
    QStringLiteral("#00f000"),   // 3=S  绿 green
    QStringLiteral("#f00000"),   // 4=Z  红 red
    QStringLiteral("#0000f0"),   // 5=J  蓝 blue
    QStringLiteral("#f0a000")    // 6=L  橙 orange
};

// 等级速度曲线 (ms) / Level speed curve
const QList<int> GameEngine::s_levelSpeeds = {
    800, 720, 630, 550, 470, 380, 290, 200, 140, 100,
    80,  70,  60,  50,  40
};

// 方块形状定义 / Piece shape definitions
const QList<QList<QList<int>>> GameEngine::s_pieceShapes = {
    /* 0=I */ {{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
    /* 1=O */ {{1,1},{1,1}},
    /* 2=T */ {{0,1,0},{1,1,1},{0,0,0}},
    /* 3=S */ {{0,1,1},{1,1,0},{0,0,0}},
    /* 4=Z */ {{1,1,0},{0,1,1},{0,0,0}},
    /* 5=J */ {{1,0,0},{1,1,1},{0,0,0}},
    /* 6=L */ {{0,0,1},{1,1,1},{0,0,0}}
};

// ============================================================
//  构造 / Constructor
// ============================================================

GameEngine::GameEngine(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &GameEngine::tick);

    initBoard();
}

// ============================================================
//  棋盘 / Board
// ============================================================

void GameEngine::initBoard()
{
    m_board.clear();
    m_board.reserve(ROWS);
    for (int r = 0; r < ROWS; ++r)
        m_board.append(QList<int>(COLS, 0));
}

QVariantList GameEngine::boardData() const
{
    QVariantList out;
    out.reserve(m_board.size());
    for (const auto &row : m_board) {
        QVariantList rowData;
        rowData.reserve(row.size());
        for (int cell : row)
            rowData.append(cell);
        out.append(rowData);
    }
    return out;
}

// ============================================================
//  单格访问器 / Per-cell accessors
//  用于 QML 渲染，避免嵌套 QVariantList 访问问题
// ============================================================

int GameEngine::boardCell(int row, int col) const
{
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS)
        return 0;
    return m_board[row][col];
}

int GameEngine::currentPieceCell(int row, int col) const
{
    if (m_currentPiece.type < 0)
        return 0;
    int r = row - m_currentPiece.y;
    int c = col - m_currentPiece.x;
    if (r < 0 || r >= m_currentPiece.shape.size())
        return 0;
    if (c < 0 || c >= m_currentPiece.shape[r].size())
        return 0;
    return m_currentPiece.shape[r][c];
}

int GameEngine::ghostCell(int row, int col) const
{
    if (m_currentPiece.type < 0)
        return 0;
    int gy = computeGhostY();
    int r = row - gy;
    int c = col - m_currentPiece.x;
    if (r < 0 || r >= m_currentPiece.shape.size())
        return 0;
    if (c < 0 || c >= m_currentPiece.shape[r].size())
        return 0;
    return m_currentPiece.shape[r][c];
}

// 高层渲染方法——封装全部游戏逻辑，QML 只需调用一次
// High-level rendering — encapsulates all game logic in one call

QString GameEngine::cellColor(int row, int col) const
{
    // 优先：当前方块 / Priority: current piece
    if (m_currentPiece.type >= 0 && !m_gameOver) {
        int r = row - m_currentPiece.y;
        int c = col - m_currentPiece.x;
        if (r >= 0 && r < m_currentPiece.shape.size()
            && c >= 0 && c < m_currentPiece.shape[r].size()
            && m_currentPiece.shape[r][c])
            return s_pieceColors[m_currentPiece.type];
    }
    // 其次：已锁定的方块 / Second: locked blocks
    if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
        int v = m_board[row][col];
        if (v > 0) return s_pieceColors[v - 1];
    }
    return QStringLiteral("transparent");
}

bool GameEngine::ghostVisible(int row, int col) const
{
    if (m_currentPiece.type < 0 || m_gameOver)
        return false;
    int gy = computeGhostY();
    int r = row - gy;
    int c = col - m_currentPiece.x;
    return (r >= 0 && r < m_currentPiece.shape.size()
            && c >= 0 && c < m_currentPiece.shape[r].size()
            && m_currentPiece.shape[r][c] != 0);
}

QString GameEngine::ghostColor() const
{
    if (m_currentPiece.type >= 0 && !m_gameOver)
        return s_pieceColors[m_currentPiece.type];
    return QStringLiteral("transparent");
}

// ============================================================
//  7-BAG 随机生成器 / 7-Bag randomiser
// ============================================================

void GameEngine::shuffleBag()
{
    m_bag = {0,1,2,3,4,5,6};
    // Fisher-Yates 洗牌
    for (int i = m_bag.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        std::swap(m_bag[i], m_bag[j]);
    }
}

int GameEngine::drawType()
{
    if (m_bag.isEmpty())
        shuffleBag();
    return m_bag.takeFirst();
}

// ============================================================
//  方块辅助 / Piece helpers
// ============================================================

int GameEngine::currentPieceType() const
{
    return m_currentPiece.type;
}

int GameEngine::currentPieceX() const
{
    return m_currentPiece.x;
}

int GameEngine::currentPieceY() const
{
    return m_currentPiece.y;
}

QVariantList GameEngine::currentPieceShape() const
{
    QVariantList out;
    for (const auto &row : m_currentPiece.shape) {
        QVariantList r;
        for (int v : row) r.append(v);
        out.append(r);
    }
    return out;
}

QVariantList GameEngine::pieceShape(int type) const
{
    if (type < 0 || type >= s_pieceShapes.size())
        return {};
    const auto &src = s_pieceShapes[type];
    QVariantList out;
    for (const auto &row : src) {
        QVariantList r;
        for (int v : row) r.append(v);
        out.append(r);
    }
    return out;
}

QString GameEngine::pieceColor(int type) const
{
    if (type < 0 || type >= s_pieceColors.size())
        return QStringLiteral("#888888");
    return s_pieceColors[type];
}

// ============================================================
//  碰撞检测 / Collision detection
// ============================================================

bool GameEngine::checkCollision(const QList<QList<int>> &shape,
                                int offX, int offY) const
{
    for (int r = 0; r < shape.size(); ++r) {
        for (int c = 0; c < shape[r].size(); ++c) {
            if (!shape[r][c]) continue;
            int bx = offX + c;
            int by = offY + r;
            if (bx < 0 || bx >= COLS || by >= ROWS)
                return true;
            if (by >= 0 && m_board[by][bx] != 0)
                return true;
        }
    }
    return false;
}

// ============================================================
//  生成 / 锁定 / 消行 / Spawn / Lock / Clear
// ============================================================

void GameEngine::spawnPiece()
{
    int type = m_nextPieceType;
    m_nextPieceType = drawType();

    m_currentPiece.type  = type;
    m_currentPiece.shape = s_pieceShapes[type];
    m_currentPiece.x     = (COLS - m_currentPiece.shape[0].size()) / 2;
    // I 方块起始位置更高 / I-piece spawns higher
    m_currentPiece.y     = (type == 0) ? -1 : 0;

    if (checkCollision(m_currentPiece.shape,
                       m_currentPiece.x, m_currentPiece.y)) {
        // 游戏结束 / Game Over
        m_gameOver = true;
        m_gameRunning = false;
        stopTimer();
        notifyAll();
    }
}

void GameEngine::lockPiece()
{
    if (m_currentPiece.type < 0) return;

    const auto &sh = m_currentPiece.shape;
    int px = m_currentPiece.x;
    int py = m_currentPiece.y;
    int typeIdx = m_currentPiece.type;

    for (int r = 0; r < sh.size(); ++r) {
        for (int c = 0; c < sh[r].size(); ++c) {
            if (!sh[r][c]) continue;
            int bx = px + c;
            int by = py + r;
            if (by >= 0 && by < ROWS && bx >= 0 && bx < COLS)
                m_board[by][bx] = typeIdx + 1;   // 1-based 存储
        }
    }

    clearLines();

    if (m_gameRunning) {
        spawnPiece();
        notifyAll();
    }
}

void GameEngine::clearLines()
{
    QList<int> fullRows;
    for (int r = ROWS - 1; r >= 0; --r) {
        bool full = true;
        for (int c = 0; c < COLS; ++c) {
            if (m_board[r][c] == 0) { full = false; break; }
        }
        if (full) fullRows.append(r);
    }

    if (fullRows.isEmpty()) return;

    // 从底部向顶部移除（保持索引有效）/ Remove bottom-to-top
    std::sort(fullRows.begin(), fullRows.end());
    for (int i = fullRows.size() - 1; i >= 0; --i)
        m_board.removeAt(fullRows[i]);
    for (int i = 0; i < fullRows.size(); ++i)
        m_board.prepend(QList<int>(COLS, 0));

    // 更新分数和等级 / Update score & level
    m_lines += fullRows.size();
    int oldLevel = m_level;
    m_level = qMin(m_lines / 10 + 1, s_levelSpeeds.size());
    updateTimer();

    static const int pts[] = {0, 100, 300, 500, 800};
    int idx = qMin(fullRows.size(), 4);
    m_score += pts[idx] * m_level;

    emit linesCleared(fullRows.size());
}

// ============================================================
//  幽灵方块 / Ghost piece
// ============================================================

int GameEngine::computeGhostY() const
{
    if (m_currentPiece.type < 0) return 0;
    int gy = m_currentPiece.y;
    while (!checkCollision(m_currentPiece.shape, m_currentPiece.x, gy + 1))
        ++gy;
    return gy;
}

int GameEngine::ghostY() const
{
    return computeGhostY();
}

// ============================================================
//  移动操作（由 QML 调用）/ Moves (invoked from QML)
// ============================================================

void GameEngine::moveLeft()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_currentPiece.type < 0)
        return;
    if (!checkCollision(m_currentPiece.shape, m_currentPiece.x - 1, m_currentPiece.y)) {
        m_currentPiece.x--;
        notifyAll();
    }
}

void GameEngine::moveRight()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_currentPiece.type < 0)
        return;
    if (!checkCollision(m_currentPiece.shape, m_currentPiece.x + 1, m_currentPiece.y)) {
        m_currentPiece.x++;
        notifyAll();
    }
}

void GameEngine::moveDown()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_currentPiece.type < 0)
        return;
    if (!checkCollision(m_currentPiece.shape, m_currentPiece.x, m_currentPiece.y + 1)) {
        m_currentPiece.y++;
        m_score += 1;      // 软降 +1 分/行 / soft drop +1 per row
        notifyAll();
    } else {
        lockPiece();
        notifyAll();
    }
}

void GameEngine::rotatePiece()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_currentPiece.type < 0)
        return;

    // 顺时针旋转 / Clockwise rotation
    const int n = m_currentPiece.shape.size();
    QList<QList<int>> rotated(n, QList<int>(n, 0));
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            rotated[r][c] = m_currentPiece.shape[n - 1 - c][r];

    // 墙踢偏移表 (8 位置) / Wall kick offsets
    static const QList<QPoint> kicks = {
        {0,0}, {-1,0}, {1,0}, {0,-1}, {-1,-1}, {1,-1}, {-2,0}, {2,0}
    };

    for (const auto &k : kicks) {
        if (!checkCollision(rotated, m_currentPiece.x + k.x(),
                            m_currentPiece.y + k.y())) {
            m_currentPiece.shape = rotated;
            m_currentPiece.x += k.x();
            m_currentPiece.y += k.y();
            notifyAll();
            return;
        }
    }
}

void GameEngine::hardDrop()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_currentPiece.type < 0)
        return;

    int d = 0;
    while (!checkCollision(m_currentPiece.shape,
                           m_currentPiece.x, m_currentPiece.y + d + 1))
        ++d;
    m_currentPiece.y += d;
    m_score += d * 2;    // 硬降 +2 分/行 / hard drop +2 per row
    lockPiece();
    notifyAll();
}

void GameEngine::togglePause()
{
    if (!m_gameRunning || m_gameOver) return;
    m_paused = !m_paused;
    if (m_paused)
        stopTimer();
    else
        startTimer();
    notifyAll();
}

// ============================================================
//  游戏生命周期 / Game lifecycle
// ============================================================

void GameEngine::startGame()
{
    // 重置所有状态 / Reset everything
    m_score       = 0;
    m_level       = 1;
    m_lines       = 0;
    m_gameOver    = false;
    m_paused      = false;

    m_currentPiece = Piece{-1, {}, 0, 0};
    m_bag.clear();

    initBoard();

    // 播种 bag 并生成首个方块 / Seed bag and spawn first piece
    shuffleBag();
    m_nextPieceType = drawType();
    spawnPiece();

    m_gameRunning = true;

    startTimer();
    notifyAll();
}

void GameEngine::restartGame()
{
    stopTimer();
    m_gameRunning = false;
    m_gameOver    = false;
    m_currentPiece = Piece{-1, {}, 0, 0};
    m_score       = 0;
    m_level       = 1;
    m_lines       = 0;
    m_bag.clear();
    initBoard();
    notifyAll();
}

// ============================================================
//  定时器（重力）/ Timer (gravity)
// ============================================================

void GameEngine::tick()
{
    if (!m_gameRunning || m_paused || m_gameOver || m_currentPiece.type < 0)
        return;

    if (!checkCollision(m_currentPiece.shape,
                        m_currentPiece.x, m_currentPiece.y + 1)) {
        m_currentPiece.y++;
        notifyAll();
    } else {
        lockPiece();
        notifyAll();
    }
}

void GameEngine::startTimer()
{
    updateTimer();
    m_timer->start();
}

void GameEngine::stopTimer()
{
    m_timer->stop();
}

void GameEngine::updateTimer()
{
    int idx = qBound(0, m_level - 1, s_levelSpeeds.size() - 1);
    m_timer->setInterval(s_levelSpeeds[idx]);
}

// ============================================================
//  通知 / Notify
// ============================================================

void GameEngine::notifyAll()
{
    m_stateRevision++;          // 递增修订号，强制 QML 绑定重算
    emit stateChanged();
}
