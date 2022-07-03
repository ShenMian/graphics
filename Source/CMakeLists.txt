# Copyright 2021 SMS
# License(Apache-2.0)

list(APPEND INCS "${DEPS_DIR}/Math/include")
list(APPEND INCS "${DEPS_DIR}/fmt/include")

find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")
list(APPEND INCS ${STB_INCLUDE_DIRS})

find_path(VULKAN_HEADERS_INCLUDE_DIRS "vk_video/vulkan_video_codec_h264std.h")
list(APPEND INCS PRIVATE ${VULKAN_HEADERS_INCLUDE_DIRS})

find_package(glfw3 CONFIG REQUIRED)
list(APPEND LIBS glfw)

find_package(imgui CONFIG REQUIRED)
list(APPEND LIBS imgui::imgui)

find_package(imguizmo CONFIG REQUIRED)
list(APPEND LIBS imguizmo::imguizmo)

find_package(meshoptimizer CONFIG REQUIRED)
list(APPEND LIBS meshoptimizer::meshoptimizer)

find_package(spirv_cross_core CONFIG REQUIRED)
list(APPEND LIBS spirv-cross-core)

# 添加 glslang
add_subdirectory("${DEPS_DIR}/glslang" "glslang")

# 添加 spirv-headers
add_subdirectory("${DEPS_DIR}/SPIRV-Headers" "SPIRV-Headers")

# 添加 spirv-tools
add_subdirectory("${DEPS_DIR}/SPIRV-Tools" "SPIRV-Tools")

# 添加 shaderc
set(SHADERC_SKIP_TESTS ON)
add_subdirectory("${DEPS_DIR}/shaderc" "shaderc")
list(APPEND LIBS shaderc)
list(APPEND INCS "${DEPS_DIR}/shaderc/libshaderc/include")

# 添加 imgui-node-editor
include("${DEPS_DIR}/imgui_node_editor.cmake")
list(APPEND LIBS imgui_node_editor)
list(APPEND INCS "${DEPS_DIR}/imgui-node-editor")

# 添加 freetype
find_package(freetype QUIET)
if(FREETYPE_FOUND)
    list(APPEND LIBS ${FREETYPE_LIBRARIES})
    list(APPEND INCS ${FREETYPE_INCLUDE_DIRS})
else()
    add_subdirectory("${DEPS_DIR}/freetype" "freetype")
    list(APPEND LIBS freetype)
    list(APPEND INCS "${DEPS_DIR}/freetype/include")
endif()

# 添加 glad
find_package(glad QUIET)
if(GLAD_FOUND)
    list(APPEND LIBS glad::glad)
else()
    include("${DEPS_DIR}/glad.cmake")
    list(APPEND LIBS glad)
endif()
list(APPEND INCS "${DEPS_DIR}/glad/include")

# 添加 assimp
# TODO: 当前 vcpkg 上的版本过旧: https://github.com/microsoft/vcpkg/pull/24843
# find_package(assimp CONFIG REQUIRED)
# list(APPEND LIBS assimp::assimp)
find_package(assimp CONFIG QUIET)
if(assimp_FOUND)
    list(APPEND LIBS assimp::assimp)
else()
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build package with shared libraries." FORCE)
    add_subdirectory("${DEPS_DIR}/assimp" "assimp")
    list(APPEND LIBS assimp)
endif()
list(APPEND INCS
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include")

#[[

# 添加 glfw3
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Scripts")
set(GLFW3_DIR "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")
find_package(GLFW3 QUIET)
# TODO: ${GLFW3_LIBRARIES} 指定的文件可能与实际架构不一致
if(GLFW3_FOUND)
    list(APPEND LIBS ${GLFW3_LIBRARIES})
else()
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation" FORCE)
    add_subdirectory("${DEPS_DIR}/glfw" "glfw3")
    list(APPEND LIBS glfw)
endif()
list(APPEND INCS "${DEPS_DIR}/glfw/include")

# 添加 imgui
find_package(imgui QUIET)
if(IMGUI_FOUND)
    list(APPEND LIBS imgui::imgui)
else()
    include("${DEPS_DIR}/imgui.cmake")
    list(APPEND LIBS imgui)
endif()
list(APPEND INCS "${DEPS_DIR}/imgui")

# 添加 imguizmo
include("${DEPS_DIR}/imguizmo.cmake")
list(APPEND LIBS imguizmo)
list(APPEND INCS "${DEPS_DIR}/ImGuizmo")

# 添加 vulkan
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    list(APPEND LIBS ${Vulkan_LIBRARIES})
elseif(WIN32)
    # FIXME: 临时解决方案
    list(APPEND LIBS "${DEPS_DIR}/VulkanSDK/Lib/vulkan-1.lib")
else()
    message(FATAL_ERROR "Vulkan not found")
endif()
list(APPEND INCS "${DEPS_DIR}/Vulkan-Headers/include")

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

]]

file(GLOB_RECURSE SOURCES "*.cpp")

# 添加静态库文件
add_library(graphics STATIC ${SOURCES})
add_library(graphics::graphics ALIAS graphics)

# 指定头文件目录
target_include_directories(graphics PUBLIC
    ${INCS}
    "${PROJECT_SOURCE_DIR}/Source")

# 指定库文件目录
target_link_libraries(graphics PUBLIC ${LIBS})

# 添加宏定义
target_compile_definitions(graphics PUBLIC ${GRAPHICS_DEFINES})

# 指定 C++ 标准
if(MSVC) # MSVC
    set_property(TARGET graphics PROPERTY
            CXX_STANDARD          23
            CXX_STANDARD_REQUIRED ON)
elseif(APPLE) # AppleClang
    target_compile_options(graphics PRIVATE -std=c++2a)
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") # GCC/Clang
    set_property(TARGET graphics PROPERTY
            CXX_STANDARD          20
            CXX_STANDARD_REQUIRED ON)
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()
