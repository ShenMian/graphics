#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

if ! hash "cmake" &>/dev/null; then
    echo "=== Need cmake."
    exit 1
fi

if [ $# -eq 0 ]; then
  BUILD_TYPE="Debug"
else
  BUILD_TYPE=$1
fi

if [ ! -z "$2" ] && [ ! -z "$3" ]; then
  CONAN_ARGS="-s compiler=$2 -s compiler.version=$3"
fi

cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd .. || exit

mkdir build 2>/dev/null

echo "=== Installing dependencies..."
export CONAN_SYSREQUIRES_MODE=enabled
if ! conan install . --build=missing -if build -of build -s build_type=${BUILD_TYPE} ${CONAN_ARGS} >/dev/null
then
  echo "=== Failed to install."
  exit 1
fi

echo "=== Generating CMake cache..."
if ! cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -Wno-dev >/dev/null
then
    echo "=== Failed to generate CMake cache."
    exit 1
fi

echo "=== Generating 'compile_commands.json'..."
cp build/compile_commands.json .

echo "=== Building..."
if ! cmake --build build --config ${BUILD_TYPE} -j16 >/dev/null
then
    echo "=== Failed to build."
    exit 1
fi

echo "=== Done."
