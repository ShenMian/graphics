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
  CONAN_ARGS="-s build_type=${BUILD_TYPE} -s compiler=$2 -s compiler.version=$3"
fi

cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd .. || exit

mkdir build 2>/dev/null

echo "=== Installing dependencies..."
export CONAN_SYSREQUIRES_MODE=enabled
conan install . --build=missing -if build -of build ${CONAN_ARGS} -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True >/dev/null || {
  echo "=== Failed to install."
  exit 1
}

echo "=== Generating CMake cache..."
cmake -B build -Wno-dev >/dev/null || {
  echo "=== Failed to generate CMake cache."
  exit 1
}

echo "=== Generating 'compile_commands.json'..."
cp build/compile_commands.json . &>/dev/null || echo "No 'compile_commands.json' was generated."

echo "=== Building..."
cmake --build build --config "${BUILD_TYPE}" -- -j$(nproc) >/dev/null || {
  echo "=== Failed to build."
  exit 1
}

echo "=== Done."
