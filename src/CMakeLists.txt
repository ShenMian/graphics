# Copyright 2022 ShenMian
# License(Apache-2.0)

############################################################################

file(GLOB_RECURSE SOURCES "*.cpp")

add_library(graphics STATIC ${SOURCES})
add_library(graphics::graphics ALIAS graphics)

target_include_directories(graphics PUBLIC
    "${INCS}"
    "${CMAKE_BINARY_DIR}/include" # TODO: maybe not necessary
    "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(graphics PUBLIC ${LIBS} ${CONAN_LIBS})
target_compile_definitions(graphics PUBLIC ${GRAPHICS_DEFINES})
target_compile_features(graphics PRIVATE cxx_std_20)

# math
target_include_directories(graphics PUBLIC
    "${DEPS_DIR}/math/src"
    "${DEPS_DIR}/math/deps/gcem/include")

# stb
target_include_directories(graphics PUBLIC "${DEPS_DIR}/stb")

# fmt
target_include_directories(graphics PUBLIC "${DEPS_DIR}/fmt/include")

# meshoptimizer
find_package(meshoptimizer CONFIG QUIET)
if(meshoptimizer_FOUND)
    target_link_libraries(graphics PRIVATE meshoptimizer::meshoptimizer)
else()
    add_subdirectory("${DEPS_DIR}/meshoptimizer" "meshoptimizer")
    target_link_libraries(graphics PRIVATE meshoptimizer)
endif()
target_include_directories(graphics PUBLIC "${DEPS_DIR}/meshoptimizer/src")

# vulkan
find_package(Vulkan QUIET)
if(Vulkan_FOUND)
    target_link_libraries(graphics PRIVATE ${Vulkan_LIBRARIES})
elseif(WIN32)
    # FIXME: 临时解决方案
    target_link_libraries(graphics PRIVATE "${DEPS_DIR}/VulkanSDK/Lib/vulkan-1.lib")
else()
    message(FATAL_ERROR "Vulkan not found")
endif()
target_include_directories(graphics PUBLIC "${DEPS_DIR}/Vulkan-Headers/include")

# imgui
find_package(imgui QUIET)
if(imgui_FOUND)
    target_link_libraries(graphics PRIVATE imgui::imgui)
else()
    include("${DEPS_DIR}/imgui.cmake")
    target_link_libraries(graphics PRIVATE imgui)
endif()
target_include_directories(graphics PUBLIC
    "${DEPS_DIR}/imgui"
    "${DEPS_DIR}/imgui/backends")

# imguizmo
include("${DEPS_DIR}/imguizmo.cmake")
target_link_libraries(graphics PRIVATE imguizmo)
target_include_directories(graphics PUBLIC "${DEPS_DIR}/ImGuizmo")

# imnodes
include("${DEPS_DIR}/imnodes.cmake")
target_link_libraries(graphics PRIVATE imnodes)
target_include_directories(graphics PUBLIC "${DEPS_DIR}/imnodes")
target_include_directories(imnodes PRIVATE
    "${DEPS_DIR}/imgui"
    "${DEPS_DIR}/imgui/backends")

# assimp
find_package(assimp QUIET)
if(assimp_FOUND)
    target_link_libraries(graphics PRIVATE assimp::assimp)
else()
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    if(WIN32)
        set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
    endif()
    add_subdirectory("${DEPS_DIR}/assimp" "assimp")
    target_link_libraries(graphics PRIVATE assimp)
endif()
target_include_directories(graphics PUBLIC
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include")

# freetype
find_package(freetype QUIET)
if(FREETYPE_FOUND)
    target_link_libraries(graphics PRIVATE ${FREETYPE_LIBRARIES})
    target_include_directories(graphics PUBLIC ${FREETYPE_INCLUDE_DIRS})
else()
    add_subdirectory("${DEPS_DIR}/freetype" "freetype")
    target_link_libraries(graphics PRIVATE freetype)
    target_include_directories(graphics PUBLIC "${DEPS_DIR}/freetype/include")
endif()

# glfw3
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/scripts")
if(WIN32)
    set(GLFW3_DIR "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")
endif()
find_package(GLFW3 QUIET)
# TODO: ${GLFW3_LIBRARIES} 指定的文件可能与实际架构不一致
if(GLFW3_FOUND)
    target_link_libraries(graphics PRIVATE ${GLFW3_LIBRARIES})
else()
    set(BUILD_SHARED_LIBS   OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL        OFF CACHE BOOL "" FORCE)
    set(GLFW_VULKAN_STATIC  OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
    add_subdirectory("${DEPS_DIR}/glfw" "glfw3")
    target_link_libraries(graphics PRIVATE glfw)
endif()
# target_link_libraries(graphics PRIVATE glfw_mt)
target_include_directories(graphics PUBLIC "${DEPS_DIR}/glfw/include")

# glad
find_package(glad QUIET)
if(GLAD_FOUND)
    target_link_libraries(graphics PRIVATE glad::glad)
else()
    include("${DEPS_DIR}/glad.cmake")
    target_link_libraries(graphics PRIVATE glad)
    target_include_directories(graphics PUBLIC "${DEPS_DIR}/glad/include")
endif()

# spirv-headers
add_subdirectory("${DEPS_DIR}/SPIRV-Headers" "SPIRV-Headers")

# spirv-tools
add_subdirectory("${DEPS_DIR}/SPIRV-Tools" "SPIRV-Tools")

# spirv-cross
find_package(spirv_cross_core QUIET)
if(NOT spirv_cross_core_FOUND)
    add_subdirectory("${DEPS_DIR}/SPIRV-Cross" "SPIRV-Cross")
endif()
target_link_libraries(graphics PRIVATE spirv-cross-core)

# glslang
# find_package(glslang QUIET)
add_subdirectory("${DEPS_DIR}/glslang" "glslang")

# shaderc
# find_package(shaderc QUIET)
# if(shaderc_FOUND)
#     list(APPEND LIBS shaderc::shaderc)
# else()
    set(SHADERC_SKIP_TESTS ON)
    add_subdirectory("${DEPS_DIR}/shaderc" "shaderc")
    target_link_libraries(graphics PRIVATE shaderc)
    target_include_directories(graphics PUBLIC "${DEPS_DIR}/shaderc/libshaderc/include")
# endif()

# xinput
if(WIN32)
    target_link_libraries(graphics PRIVATE xinput)
endif()

