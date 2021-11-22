# 指定最小版本号
cmake_minimum_required(VERSION 3.16)

# 指定项目名和版本号
project(Graphics VERSION 0.0.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR "${PROJECT_SOURCE_DIR}/..")
set(OUTPUT_DIR       "${PROJECT_SOURCE_DIR}/build")

set(LIBRARY_OUTPUT_PATH    "${OUTPUT_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_DIR}/bin")

macro(ADD_DEFINE IDENT)
    add_definitions("-D${IDENT}")
endmacro()

if(WIN32)
    ADD_DEFINE(_CRT_SECURE_NO_WARNINGS)
    ADD_DEFINE(_SCL_SECURE_NO_WARNINGS)
    ADD_DEFINE(UNICODE)
    ADD_DEFINE(_UNICODE)
    ADD_DEFINE(NOMINMAX)
endif(WIN32)

add_subdirectory(Source)
# add_subdirectory(Examples/Triangle)
