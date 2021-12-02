# Copyright 2021 SMS
# License(Apache-2.0)

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

target_link_libraries(graphics PUBLIC glad glfw assimp::assimp meshoptimizer::meshoptimizer)
