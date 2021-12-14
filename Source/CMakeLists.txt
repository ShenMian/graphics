# Copyright 2021 SMS
# License(Apache-2.0)

list(APPEND INCS
    "${DEPS_DIR}/stb"
    "${DEPS_DIR}/Math/include")

include("${DEPS_DIR}/glad.cmake")
list(APPEND LIBS glad)
list(APPEND INCS "${DEPS_DIR}/glad/include")

set(glfw3_DIR "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Scripts")
find_package(GLFW3)
if(glfw3_FOUND)
    list(APPEND LIBS ${glfw3_LIBRARIES})
else()
    add_subdirectory("${DEPS_DIR}/glfw" "glfw3")
    list(APPEND LIBS glfw)
endif()
list(APPEND INCS "${DEPS_DIR}/glfw/include")

find_package(assimp CONFIG)
if(assimp_FOUND)
    list(APPEND LIBS ${assimp_LIBRARIES})
else()
    add_subdirectory("${DEPS_DIR}/assimp" "assimp")
    list(APPEND LIBS assimp)
endif()
list(APPEND INCS
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include")

find_package(meshoptimizer CONFIG)
if(meshoptimizer_FOUND)
    list(APPEND LIBS ${meshoptimizer_LIBRARIES})
else()
    add_subdirectory("${DEPS_DIR}/meshoptimizer" "meshoptimizer")
    list(APPEND LIBS meshoptimizer)
endif()
list(APPEND INCS "${DEPS_DIR}/meshoptimizer/src")

include_directories(
    ${INCS}
    "${PROJECT_SOURCE_DIR}/Source"
    "${DEPS_DIR}/Vulkan-Headers/include"
    "${DEPS_DIR}/meshoptimizer/src")

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(graphics STATIC ${SOURCES})

target_link_libraries(graphics PUBLIC ${LIBS})
