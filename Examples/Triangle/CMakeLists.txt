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

add_executable(main main.cpp)

target_link_libraries(main PRIVATE graphics glad)

file(COPY "Shaders" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
