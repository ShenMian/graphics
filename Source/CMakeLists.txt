# Copyright 2021 SMS
# License(Apache-2.0)

list(APPEND LIBS glad glfw)

find_package(assimp CONFIG)
if(assimp_FOUND)
    list(APPEND LIBS assimp::assimp)
else()
    add_subdirectory("${THIRDPARTY_DIR}/assimp")
    list(APPEND LIBS assimp)
endif()

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

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(graphics STATIC ${SOURCES})

if(USE_CMAKE_PACKAGE)
    target_link_libraries(graphics PUBLIC ${LIBS} meshoptimizer::meshoptimizer)
else()
    target_link_libraries(graphics PUBLIC ${LIBS} meshoptimizer)
endif()
