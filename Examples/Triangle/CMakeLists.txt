# Copyright 2021 SMS
# License(Apache-2.0)

include_directories(
    "${PROJECT_SOURCE_DIR}/Source"
    "${DEPS_DIR}/stb"
    "${DEPS_DIR}/Math/include"
    "${DEPS_DIR}/glad/include"
    "${DEPS_DIR}/glfw/include"
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include"
    "${DEPS_DIR}/Vulkan-Headers/include"
    "${DEPS_DIR}/meshoptimizer/src")

add_executable(main main.cpp)

target_link_libraries(main PRIVATE graphics glad)

file(COPY "Shaders" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
