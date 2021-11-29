# Copyright 2021 SMS
# License(Apache-2.0)

set(THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")

include_directories(
    "${PROJECT_SOURCE_DIR}/Source"
    "${THIRDPARTY_DIR}/stb"
    "${THIRDPARTY_DIR}/Math/include"
    "${THIRDPARTY_DIR}/glad/include"
    "${THIRDPARTY_DIR}/glfw/include"
    "${THIRDPARTY_DIR}/assimp/build/include/assimp"
    "${THIRDPARTY_DIR}/Vulkan-Headers/include"
    "${THIRDPARTY_DIR}/meshoptimizer/src")

add_executable(main main.cpp)

target_link_libraries(main PRIVATE Graphics assimp glad)

file(COPY Shaders DESTINATION ".")
