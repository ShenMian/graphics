# Copyright 2021 SMS
# License(Apache-2.0)

list(APPEND INCS
    "${DEPS_DIR}/stb"
    "${DEPS_DIR}/Math/include"
    "${DEPS_DIR}/glad/include"
    "${DEPS_DIR}/glfw/include")

list(APPEND LIBS
    glad
    glfw)

find_package(assimp CONFIG)
if(assimp_FOUND)
    list(APPEND INCS ${assimp_INCLUDE_DIRS})
    list(APPEND LIBS ${assimp_LIBRARIES})
else()
    add_subdirectory("${DEPS_DIR}/assimp")
    list(APPEND INCS
        "${DEPS_DIR}/assimp/include"
        "${DEPS_DIR}/assimp/build/include")
    list(APPEND LIBS assimp)
endif()

include_directories(
    ${INCS}
    "${PROJECT_SOURCE_DIR}/Source"
    "${DEPS_DIR}/Vulkan-Headers/include"
    "${DEPS_DIR}/meshoptimizer/src")

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(graphics STATIC ${SOURCES})

if(USE_CMAKE_PACKAGE)
    target_link_libraries(graphics PUBLIC ${LIBS} meshoptimizer::meshoptimizer)
else()
    target_link_libraries(graphics PUBLIC ${LIBS} meshoptimizer)
endif()
