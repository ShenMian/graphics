# Copyright 2022 ShenMian
# License(Apache-2.0)

if(NOT USE_CONAN)
    list(APPEND INCS "${DEPS_DIR}/stb")
    list(APPEND INCS "${DEPS_DIR}/fmt/include")

    # imgui
    find_package(imgui QUIET)
    if(imgui_FOUND)
        list(APPEND LIBS imgui::imgui)
    else()
        include("${DEPS_DIR}/imgui.cmake")
        list(APPEND LIBS imgui)
    endif()
    list(APPEND INCS "${DEPS_DIR}/imgui")
    list(APPEND INCS "${DEPS_DIR}/imgui/backends")

    # imguizmo
    include("${DEPS_DIR}/imguizmo.cmake")
    list(APPEND LIBS imguizmo)
    list(APPEND INCS "${DEPS_DIR}/ImGuizmo")

    # freetype
    find_package(freetype QUIET)
    if(FREETYPE_FOUND)
        list(APPEND LIBS ${FREETYPE_LIBRARIES})
        list(APPEND INCS ${FREETYPE_INCLUDE_DIRS})
    else()
        add_subdirectory("${DEPS_DIR}/freetype" "freetype")
        list(APPEND LIBS freetype)
        list(APPEND INCS "${DEPS_DIR}/freetype/include")
    endif()

    # glfw3
    list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/Scripts")
    set(GLFW3_DIR "C:/Program Files (x86)/GLFW/lib/cmake/glfw3")
    find_package(GLFW3 QUIET)
    # TODO: ${GLFW3_LIBRARIES} 指定的文件可能与实际架构不一致
    if(GLFW3_FOUND)
        list(APPEND LIBS ${GLFW3_LIBRARIES})
    else()
        set(BUILD_SHARED_LIBS   OFF CACHE BOOL "Build shared libraries" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" FORCE)
        set(GLFW_BUILD_TESTS    OFF CACHE BOOL "Build the GLFW test programs" FORCE)
        set(GLFW_BUILD_DOCS     OFF CACHE BOOL "Build the GLFW documentation" FORCE)
        set(GLFW_INSTALL        OFF CACHE BOOL "Generate installation target" FORCE)
        set(GLFW_VULKAN_STATIC  OFF CACHE BOOL "Assume the Vulkan loader is linked with the application" FORCE)
        set(GLFW_BUILD_DOCS     OFF CACHE BOOL "Build the GLFW documentation" FORCE)
        add_subdirectory("${DEPS_DIR}/glfw" "glfw3")
        # list(APPEND LIBS glfw)
    endif()
    list(APPEND LIBS glfw_mt)
    list(APPEND INCS "${DEPS_DIR}/glfw/include")

    # meshoptimizer
    find_package(meshoptimizer CONFIG QUIET)
    if(meshoptimizer_FOUND)
        list(APPEND LIBS meshoptimizer::meshoptimizer)
    else()
        add_subdirectory("${DEPS_DIR}/meshoptimizer" "meshoptimizer")
        list(APPEND LIBS meshoptimizer)
    endif()
    list(APPEND INCS "${DEPS_DIR}/meshoptimizer/src")

    # vulkan
    find_package(Vulkan QUIET)
    if(Vulkan_FOUND)
        list(APPEND LIBS ${Vulkan_LIBRARIES})
    elseif(WIN32)
        # FIXME: 临时解决方案
        list(APPEND LIBS "${DEPS_DIR}/VulkanSDK/Lib/vulkan-1.lib")
    else()
        message(FATAL_ERROR "Vulkan not found")
    endif()
    list(APPEND INCS "${DEPS_DIR}/Vulkan-Headers/include")
endif()

list(APPEND INCS "${DEPS_DIR}/math/src")
list(APPEND INCS "${DEPS_DIR}/math/deps/gcem/include")
list(APPEND INCS "${CMAKE_BINARY_DIR}/include")

# assimp
find_package(assimp QUIET)
if(assimp_FOUND)
    list(APPEND LIBS assimp::assimp)
else()
    set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS         OFF CACHE BOOL "" FORCE)
    add_subdirectory("${DEPS_DIR}/assimp" "assimp")
    list(APPEND LIBS assimp)
endif()
list(APPEND INCS
    "${DEPS_DIR}/assimp/include"
    "${DEPS_DIR}/assimp/build/include")

# glad
find_package(glad QUIET)
if(GLAD_FOUND)
    list(APPEND LIBS glad::glad)
else()
    include("${DEPS_DIR}/glad.cmake")
    list(APPEND LIBS glad)
    list(APPEND INCS "${DEPS_DIR}/glad/include")
endif()

# spirv-headers
add_subdirectory("${DEPS_DIR}/SPIRV-Headers" "SPIRV-Headers")

# spirv-tools
add_subdirectory("${DEPS_DIR}/SPIRV-Tools" "SPIRV-Tools")

# spirv-cross
find_package(spirv_cross_core QUIET)
if(spirv_cross_core_FOUND)
    list(APPEND LIBS spirv-cross-core)
else()
    add_subdirectory("${DEPS_DIR}/SPIRV-Cross" "SPIRV-Cross")
    list(APPEND LIBS spirv-cross-core)
    list(APPEND INCS "${DEPS_DIR}/SPIRV-Cross")
endif()

# glslang
find_package(glslang QUIET)
add_subdirectory("${DEPS_DIR}/glslang" "glslang")

# shaderc
# find_package(shaderc QUIET)
# if(shaderc_FOUND)
#     list(APPEND LIBS shaderc::shaderc)
# else()
    set(SHADERC_SKIP_TESTS ON)
    add_subdirectory("${DEPS_DIR}/shaderc" "shaderc")
    list(APPEND LIBS shaderc)
    list(APPEND INCS "${DEPS_DIR}/shaderc/libshaderc/include")
# endif()

# imnodes
include("${DEPS_DIR}/imnodes.cmake")
list(APPEND LIBS imnodes)
list(APPEND INCS "${DEPS_DIR}/imnodes")
target_include_directories(imnodes PRIVATE
    "${DEPS_DIR}/imgui"
    "${DEPS_DIR}/imgui/backends")

if(WIN32)
list(APPEND LIBS xinput)
endif()

message(STATUS "INCS: ${INCS}")
message(STATUS "LIBS: ${LIBS}")

file(GLOB_RECURSE SOURCES "*.cpp")

if(USE_CONAN)
    list(APPEND SOURCES "${CMAKE_BINARY_DIR}/include/imgui_impl_glfw.cpp")
    list(APPEND SOURCES "${CMAKE_BINARY_DIR}/include/imgui_impl_opengl3.cpp")
    list(APPEND SOURCES "${CMAKE_BINARY_DIR}/include/imgui_impl_vulkan.cpp")
endif()

add_library(graphics STATIC ${SOURCES})
add_library(graphics::graphics ALIAS graphics)

target_include_directories(graphics PUBLIC
    "${INCS}"
    "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(graphics PUBLIC ${LIBS} ${CONAN_LIBS})
target_compile_definitions(graphics PUBLIC ${GRAPHICS_DEFINES})
target_compile_features(graphics PRIVATE cxx_std_20)
