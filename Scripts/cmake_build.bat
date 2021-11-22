@echo off
REM Copyright 2021 ShenMian
REM License(Apache-2.0)

REM 签出第三方库
git submodule update --init

REM 安装依赖项
call cmake_install_deps || exit /b 1

REM 切换到主目录
pushd %~dp0\..

REM 創建 build 目錄
if not exist build mkdir build
pushd build

REM 生成 CMake 緩存
echo Generating CMake cache...
cmake .. >NUL || (
    echo Failed to generate CMake cache.
    exit /b 1
)

REM 構建
echo Building...
cmake --build . >NUL || (
    echo Failed to build.
    exit /b 1
)

echo Done

popd

popd
