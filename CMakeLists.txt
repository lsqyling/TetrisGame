cmake_minimum_required(VERSION 3.25)

project(TetrisGame VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# 指向你的 Qt 6.8+ 安装路径 / Point to your Qt 6.8+ installation
set(CMAKE_PREFIX_PATH  D:/dev/tools/Qt/6.8.3/msvc2022_64)

# 编译选项 / Compile options
include(cmake/compile_options.cmake)

find_package(Qt6 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup(REQUIRES 6.8)

set(target_name TetrisGame)

# --- C++ 源文件 / C++ sources ---
set(cpp_files
    main.cpp
    src/GameEngine.h
    src/GameEngine.cpp
    src/AudioEngine.h
    src/AudioEngine.cpp
)

qt_add_executable(${target_name} ${cpp_files})

# --- 收集 QML 文件 / Collect QML files ---
file(GLOB_RECURSE qml_files RELATIVE ${CMAKE_CURRENT_LIST_DIR} CONFIGURE_DEPENDS qml/*.qml)

# 提取 TetrisConfig.qml 为独立单例模块 / Extract as separate singleton module
set(config_file "")
foreach(f IN LISTS qml_files)
    if(f MATCHES "([^/]+/)*TetrisConfig\\.qml$")
        set(config_file "${f}")
        break()
    endif()
endforeach()
if(config_file)
    list(REMOVE_ITEM qml_files "${config_file}")
endif()

# --- 主 QML 模块 / Main QML module ---
qt_add_qml_module(${target_name}
    URI TetrisGame
    VERSION 1.0
    QML_FILES ${qml_files}
)

# --- 配置单例模块 / Config singleton module ---
qt_add_library(TetrisConfig STATIC)
set_source_files_properties(${config_file}
    PROPERTIES
    QT_QML_SINGLETON_TYPE true
)

qt_add_qml_module(TetrisConfig
    URI "TetrisConfig"
    OUTPUT_DIRECTORY TetrisConfig
    QML_FILES ${config_file}
)

# --- 链接依赖 / Link dependencies ---
target_include_directories(${target_name} PRIVATE src)

target_link_libraries(${target_name} PRIVATE
    Qt6::Core
    Qt6::Quick
    Qt6::Multimedia
    TetrisConfigplugin
)

# 部署辅助 / Deployment helpers
include(cmake/qt6_runtime_dependency.cmake)
include(cmake/install.cmake)
