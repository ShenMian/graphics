@echo off
REM Copyright 2021 ShenMian
REM License(Apache-2.0)

cmake --version >nul 2>&1 || (
    echo === Need cmake
    exit /b 1
)

if "%~1"=="" (set "BUILD_TYPE=Debug") else (set "BUILD_TYPE=%~1")
if "%BUILD_TYPE%"=="Debug" (set "VS_ARGS=MTd") else (set "VS_ARGS=MT")

pushd %~dp0\..

echo === Installing dependencies...
mkdir build 2>nul
pushd build

set "CONAN_SYSREQUIRES_MODE=enabled"
conan install .. --build=missing -s build_type=%BUILD_TYPE% -s compiler.runtime=%VS_ARGS% >nul

popd

echo === Generating CMake cache...
cmake -B build >nul || (
    echo === Failed to generate CMake cache.
    cmake -B build
    exit /b 1
)

echo === Building...
cmake --build build >nul || (
    echo === Failed to build.
    cmake --build build --config %BUILD_TYPE% -j16
    exit /b 1
)

echo === Done.

popd
