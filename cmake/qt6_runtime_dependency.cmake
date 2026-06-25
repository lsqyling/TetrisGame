# ============================================================
#  windeployqt6 — 自动部署 Qt 运行时依赖
#  Post-build: 扫描 QML 导入 → 复制所需 DLL 和 QML 文件
# ============================================================

set(qt6_runtime_command "${CMAKE_PREFIX_PATH}/bin/windeployqt6${CMAKE_EXECUTABLE_SUFFIX}")
set(qt6_qml_source_dir "${CMAKE_CURRENT_SOURCE_DIR}/qml")

add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND ${qt6_runtime_command} --qmldir ${qt6_qml_source_dir} "${target_name}${CMAKE_EXECUTABLE_SUFFIX}"
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:${target_name}>")
