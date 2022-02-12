# Copyright 2021 SMS
# License(Apache-2.0)

list(APPEND INCS
    "${DEPS_DIR}/stb"
    "${DEPS_DIR}/Math/include")

# 添加 fmt
list(APPEND INCS "${DEPS_DIR}/fmt/include")

# 添加 Glad
include("${DEPS_DIR}/glad.cmake")
list(APPEND LIBS glad)
list(APPEND INCS "${DEPS_DIR}/glad/include")

# 添加 GLFW3
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Scripts")
set(GLFW3_DIR "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")
find_package(GLFW3 QUIET)
if(GLFW3_FOUND)
    list(APPEND LIBS ${GLFW3_LIBRARIES})
else()
    add_subdirectory("${DEPS_DIR}/glfw" "glfw3")
    list(APPEND LIBS glfw)
endif()
list(APPEND INCS "${DEPS_DIR}/glfw/include")

# 添加 imgui
include("${DEPS_DIR}/imgui.cmake")
list(APPEND LIBS imgui)
list(APPEND INCS "${DEPS_DIR}/imgui")

# 添加 ImGuizmo
include("${DEPS_DIR}/imguizmo.cmake")
list(APPEND LIBS imguizmo)
list(APPEND INCS "${DEPS_DIR}/ImGuizmo")

# 添加 imgui-node-editor
include("${DEPS_DIR}/imgui_node_editor.cmake")
list(APPEND LIBS imgui_node_editor)
list(APPEND INCS "${DEPS_DIR}/imgui-node-editor")

# 添加 assimp
find_package(assimp CONFIG QUIET)
if(assimp_FOUND)
    list(APPEND LIBS ${ASSIMP_LIBRARIES})
else()
    add_subdirectory("${DEPS_DIR}/assimp" "assimp")
    list(APPEND LIBS assimp)
endif()
list(APPEND INCS
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include")

# 添加 Vulkan
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    list(APPEND LIBS ${Vulkan_LIBRARIES})
elseif(WIN32)
    list(APPEND LIBS "${DEPS_DIR}/VulkanSDK/Lib/vulkan-1.lib")
else()
    message(FATAL_ERROR "Vulkan not found")
endif()

# 添加 vk-bootstrap
# 在 Github Action 中, 防止 vk-bootstrap 使用 NV 版本头文件, 导致编译失败
if(UBUNTU)
    add_subdirectory("${DEPS_DIR}/Vulkan-Headers" "Vulkan-Headers")
endif()
add_subdirectory("${DEPS_DIR}/vk-bootstrap" "vk-bootstrap")
list(APPEND LIBS vk-bootstrap::vk-bootstrap)

# 添加 meshoptimizer
find_package(meshoptimizer CONFIG QUIET)
if(meshoptimizer_FOUND)
    list(APPEND LIBS meshoptimizer::meshoptimizer)
else()
    add_subdirectory("${DEPS_DIR}/meshoptimizer" "meshoptimizer")
    list(APPEND LIBS meshoptimizer)
endif()
list(APPEND INCS "${DEPS_DIR}/meshoptimizer/src")

message(STATUS "INCS: ${INCS}")
message(STATUS "LIBS: ${LIBS}")


file(GLOB_RECURSE SOURCES "*.cpp")

# 添加静态库文件
add_library(graphics STATIC ${SOURCES})
add_library(graphics::graphics ALIAS graphics)

# 指定头文件目录
target_include_directories(graphics PUBLIC
    ${INCS}
    "${PROJECT_SOURCE_DIR}/Source"
    "${DEPS_DIR}/Vulkan-Headers/include"
    "${DEPS_DIR}/meshoptimizer/src")

# 指定库文件目录
target_link_libraries(graphics PUBLIC ${LIBS})

# 添加宏定义
target_compile_definitions(graphics PUBLIC ${GRAPHICS_DEFINES})

# 指定 C++ 标准
if(MSVC)
    set_property(TARGET graphics PROPERTY
        CXX_STANDARD          23
        CXX_STANDARD_REQUIRED ON) # MSVC
elseif(APPLE)
    target_compile_options(graphics PRIVATE -std=c++2a) # AppleClang
else()
    set_property(TARGET graphics PROPERTY
            CXX_STANDARD          20
            CXX_STANDARD_REQUIRED ON) # GCC/Clang
endif()
