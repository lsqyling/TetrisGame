// ============================================================
//  俄罗斯方块 — Classic Tetris
//  入口点 / Entry point
//  注册 C++ 后端单例，加载 QML 主模块
// ============================================================
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include "GameEngine.h"
#include "AudioEngine.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setApplicationName(QStringLiteral("TetrisGame"));
    app.setApplicationVersion(QStringLiteral("1.0.0"));

    // C++ 后端对象（必须在 QQmlApplicationEngine 之前创建以保活）
    // Backends must outlive the QML engine
    GameEngine  gameEngine;
    AudioEngine audioEngine;

    // 以 QML 单例形式注册（Qt 6 模块安全方式）
    // Register as QML singletons (Qt 6 module-safe approach)
    qmlRegisterSingletonInstance("GameBackend", 1, 0, "GameEngine",  &gameEngine);
    qmlRegisterSingletonInstance("GameBackend", 1, 0, "AudioEngine", &audioEngine);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    // 从 QML 模块加载主窗口
    // Load main window from the QML module
    engine.loadFromModule("TetrisGame", "Main");

    return app.exec();
}
