# ============================================================
#  编译选项 / Compile options
#  Debug: 启用调试控制台 + 插件日志
#  MSVC:  UTF-8 源文件编码
# ============================================================

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif ()
if (CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
    set(QT_WIN_DEBUG_CONSOLE new)      # 启用 Windows 调试控制台
    set(QT_DEBUG_PLUGINS 1)            # 启用 Qt 插件调试日志
    set(QML_IMPORT_TRACE 1)            # 启用 QML 导入追踪
endif ()

if (MSVC)
    add_compile_options(/utf-8)
endif ()
