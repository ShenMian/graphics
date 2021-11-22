@echo off
REM Copyright 2021 ShenMian
REM License(Apache-2.0)

net.exe session 1>NUL 2>NUL || (
    echo Please run this script with administrator privilege.
    exit /b 1
)

pushd %~dp0\..\ThirdParty

REM 签出第三方库
git submodule update --init

echo Installing dependencies...

for %%i in ("assimp" "glfw" "meshoptimizer") do (
    echo ^|-Installing '%%~i'...

    pushd %%i

    REM 創建 build 目錄
    if not exist build mkdir build
    cd build

    REM 生成 CMake 緩存
    echo  ^|-Gerenating CMake cache...
    cmake .. >NUL || (
        echo  ^|-Failed to generate CMake cache.
        exit /b 1
    )

    REM 构建
    echo  ^|-Building...
    cmake --build . --config Release >NUL || (
        echo  ^|-Failed to build.
        exit /b 1
    )

    REM 安装
    echo  ^|-Installing...
    cmake --install . >NUL || (
        echo  ^|-Failed to install.
        exit /b 1
    )

    echo  ^|-Done

    popd
)

popd
