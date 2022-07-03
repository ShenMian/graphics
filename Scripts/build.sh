#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd ..

echo Installing dependencies...
mkdir build 2>/dev/null
cd build
export CONAN_SYSREQUIRES_MODE=enabled
conan install .. --build=missing >/dev/null
cd ..

echo Generating CMake cache...
if ! cmake -B build >/dev/null
then
    echo Failed to generate CMake cache.
    cmake -B build
    exit 1
fi

echo Building...
if ! cmake --build build >/dev/null
then
    echo Failed to build.
    cmake --build build -j8
    exit 1
fi

echo Done.

