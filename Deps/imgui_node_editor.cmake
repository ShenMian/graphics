file(GLOB SOURCES "*.cpp")
add_library(imgui_node_editor STATIC ${SOURCES})
target_include_directories(imgui_node_editor PRIVATE
    "${DEPS_DIR}/imgui-node-editor"
    "${DEPS_DIR}/imgui")
