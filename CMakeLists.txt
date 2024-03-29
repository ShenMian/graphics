# Copyright 2022 ShenMian
# License(Apache-2.0)

cmake_minimum_required(VERSION 3.16)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(NOT DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()

project(Graphics
    VERSION      0.0.0
    DESCRIPTION  "A simple abstraction layer for the modern graphics APIs."
    HOMEPAGE_URL "https://github.com/ShenMian/graphics"
    LANGUAGES    CXX C)

option(BUILD_EXAMPLES "Build examples" ON)

# 输出基本信息
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.22.0")
    cmake_host_system_information(RESULT OS_PRETTY_NAME QUERY DISTRIB_PRETTY_NAME)
endif()
message(STATUS "OS")
message(STATUS "|- Name   : ${CMAKE_SYSTEM_NAME}(${OS_PRETTY_NAME})")
message(STATUS "`- Version: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Compiler")
message(STATUS "|- ID     : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "`- Version: ${CMAKE_CXX_COMPILER_VERSION}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/lib")

macro(add_example NAME)
    file(GLOB_RECURSE SOURCES "*.cpp")

    add_executable(${NAME} ${SOURCES})

    target_link_libraries(${NAME} PRIVATE graphics)
    target_compile_definitions(${NAME} PUBLIC ${GRAPHICS_DEFINES})

    set_target_properties(${NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY                "${CMAKE_BINARY_DIR}/bin/${NAME}"
        RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_BINARY_DIR}/bin/${NAME}"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_BINARY_DIR}/bin/${NAME}"
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/bin/${NAME}"
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_BINARY_DIR}/bin/${NAME}")

    # 指定 C++ 标准
    if(MSVC) # MSVC
        set_property(TARGET ${NAME} PROPERTY
            CXX_STANDARD          23
            CXX_STANDARD_REQUIRED ON)
    elseif(APPLE) # AppleClang
        target_compile_options(${NAME} PRIVATE -std=c++2a)
    elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU") # GCC/Clang
        set_property(TARGET ${NAME} PROPERTY
            CXX_STANDARD          20
            CXX_STANDARD_REQUIRED ON)
    else()
        message(FATAL_ERROR "Unsupported compiler")
    endif()

    # 将 shader 源文件复制到可执行文件的目录下
    file(COPY "shaders" DESTINATION "${CMAKE_BINARY_DIR}/bin/${NAME}")
endmacro()

if(WIN32)
    list(APPEND GRAPHICS_DEFINES _CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS UNICODE _UNICODE NOMINMAX) # 指定 Win32 下要定义的宏
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # 静态链接 MSVC 运行库
endif()

if(UNIX)
    if(OS_PRETTY_NAME MATCHES "Ubuntu")
        set(UBUNTU true)
    endif()
    add_link_options("-ldl" "-lpthread")
endif()

# 添加第三方库
set(DEPS_DIR "${PROJECT_SOURCE_DIR}/deps")

add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples/triangle)
    add_subdirectory(examples/cube)
    add_subdirectory(examples/viewer)
endif()
