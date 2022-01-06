# Copyright 2021 SMS
# License(Apache-2.0)

add_executable(cube main.cpp)

target_include_directories(cube PUBLIC
    "${PROJECT_SOURCE_DIR}/Source"
    "${DEPS_DIR}/stb"
    "${DEPS_DIR}/Math/include"
    "${DEPS_DIR}/glad/include"
    "${DEPS_DIR}/glfw/include"
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include"
    "${DEPS_DIR}/Vulkan-Headers/include"
    "${DEPS_DIR}/meshoptimizer/src")
target_link_libraries(cube PRIVATE graphics glad)
target_compile_definitions(cube PUBLIC ${GRAPHICS_DEFINES})

# 指定 C++ 标准
if(MSVC)
    set_property(TARGET cube PROPERTY
        CXX_STANDARD          23
        CXX_STANDARD_REQUIRED ON)
elseif(APPLE)
    target_compile_options(cube PRIVATE -std=c++2a)
elseif(UNIX)
    target_compile_options(cube PRIVATE -std=c++20)
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()

# 将 Shader 源文件复制到可执行文件的目录下
file(COPY "Shaders" DESTINATION "${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}")
