set(THIRDPARTY_DIR "${PROJECT_SOURCE_DIR}/ThirdParty")

# include("${THIRDPARTY_DIR}/glad.cmake")
# add_subdirectory("${THIRDPARTY_DIR}/glfw" glfw/lib)
# add_subdirectory("${THIRDPARTY_DIR}/assimp" assimp/lib)
# add_subdirectory("${THIRDPARTY_DIR}/meshoptimizer" meshoptimizer/lib)

# find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

file(GLOB_RECURSE sources "*.cpp")

include_directories(
    "${PROJECT_SOURCE_DIR}/Source"
    "${THIRDPARTY_DIR}/stb"
    "${THIRDPARTY_DIR}/Math/include"
    "${THIRDPARTY_DIR}/glad/include"
    "${THIRDPARTY_DIR}/glfw/include"
    "${THIRDPARTY_DIR}/assimp/include"
    "${THIRDPARTY_DIR}/Vulkan-Headers/include"
    "${THIRDPARTY_DIR}/meshoptimizer/src")

add_library(graphics STATIC ${sources})

target_link_libraries(graphics PRIVATE glfw glad::glad assimp::assimp)
