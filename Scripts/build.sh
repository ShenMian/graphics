#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

if ! hash "cmake" &>/dev/null; then
    echo === Need cmake
    exit 1
fi

if [ $# -eq 0 ]
    BUILD_TYPE="Debug"
else
    BUILD_TYPE=$1
fi

cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd .. || exit

echo === Installing dependencies...
mkdir build 2>/dev/null
cd build
export CONAN_SYSREQUIRES_MODE=enabled
conan install .. --build=missing -s build_type=${BUILD_TYPE} >/dev/null
cd .. || exit

echo === Generating CMake cache...
if ! cmake -B build >/dev/null
then
    echo === Failed to generate CMake cache.
    cmake -B build
    exit 1
fi

echo === Building...
if ! cmake --build build >/dev/null
then
    echo === Failed to build.
    cmake --build build --config ${BUILD_TYPE} -j16
    exit 1
fi

echo === Done.
