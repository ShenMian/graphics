@echo off
REM Copyright 2021 ShenMian
REM License(Apache-2.0)

pushd %~dp0\..

echo Installing dependencies...
mkdir build 2>nul
pushd  build
set CONAN_SYSREQUIRES_MODE=enabled
conan install .. --build=missing >nul
popd

echo Generating CMake cache...
cmake -B build >nul || (
    echo Failed to generate CMake cache.
    cmake -B build
    exit /b 1
)

echo Building...
cmake --build build >nul || (
    echo Failed to build.
    cmake --build build -j 8
    exit /b 1
)

echo Done.

popd

