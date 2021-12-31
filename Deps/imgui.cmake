
add_library(imgui STATIC
    "${DEPS_DIR}/imgui/imgui.cpp"
    "${DEPS_DIR}/imgui/imgui_demo.cpp"
    "${DEPS_DIR}/imgui/imgui_draw.cpp"
    "${DEPS_DIR}/imgui/imgui_tables.cpp"
    "${DEPS_DIR}/imgui/imgui_widgets.cpp"
    "${DEPS_DIR}/imgui/backends/imgui_impl_glfw.cpp"
    "${DEPS_DIR}/imgui/backends/imgui_impl_opengl3.cpp")
target_include_directories(imgui PUBLIC
    "${DEPS_DIR}/imgui"
    "${DEPS_DIR}/glad/include"
    "${DEPS_DIR}/glfw/include"
    "${DEPS_DIR}/Vulkan-Headers/include")
