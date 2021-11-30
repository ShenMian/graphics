# Copyright 2021 SMS
# License(Apache-2.0)

set(THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")

include_directories(
    "${PROJECT_SOURCE_DIR}/Source"
    "${THIRDPARTY_DIR}/stb"
    "${THIRDPARTY_DIR}/Math/include"
    "${THIRDPARTY_DIR}/glad/include"
    "${THIRDPARTY_DIR}/glfw/include"
    "${THIRDPARTY_DIR}/assimp/include"
    "${THIRDPARTY_DIR}/assimp/build/include"
    "${THIRDPARTY_DIR}/Vulkan-Headers/include"
    "${THIRDPARTY_DIR}/meshoptimizer/src")

file(GLOB_RECURSE sources "*.cpp")

add_library(graphics STATIC ${sources})

# TODO: 添加 FindGLFW3.cmake
set(glfw3_DIR "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")

# find_package(glad CONFIG REQUIRED)
include("${THIRDPARTY_DIR}/glad.cmake")
find_package(glfw3         CONFIG REQUIRED)
find_package(assimp        CONFIG REQUIRED)
find_package(meshoptimizer CONFIG REQUIRED)

target_link_libraries(graphics PRIVATE glad glfw assimp::assimp meshoptimizer::meshoptimizer)
