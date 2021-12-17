#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

function install_macos() {
    deps=$*

    if ! brew install $deps; then
        echo "apt install failed"
        exit 1
    fi
}

function install_ubuntu() {
    deps=$*

    if ! sudo apt install -y $deps; then
        echo "apt install failed"
        exit 1
    fi
}

function install_cmake() {
    dep=$1

    cd "${dep}" || return 1

    echo "|-Installing '${dep}'..."

    # 生成 CMake 緩存
    echo " |-Gerenating CMake cache..."
    if ! cmake -B build >/dev/null 2>&1
    then
        echo " |-Failed to generate CMake cache."
        cmake -B build
        return 1
    fi

    # 构建
    echo " |-Building..."
    if ! cmake --build build --config Release >/dev/null
    then
        echo " |-Failed to build."
        cmake --build build --config Release
        return 1
    fi

    # 安装
    echo  " |-Installing..."
    if ! sudo cmake --install build >/dev/null
    then
        echo " |-Failed to install."
        return 1
    fi

    echo " |-Done."

    cd ..
}

# 切换到 Deps 目录
cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd ../Deps

# 獲取管理員權限
if ! sudo echo
then
    echo "Can not get sudo permission."
    exit 1
fi

# 签出第三方库
echo Checkout third-party libraries...
if ! git submodule update --init >/dev/null
then
    echo Failed to checkout third-party libraries.
    exit 1
fi

echo Installing dependencies...

# Ubuntu
if sudo apt update >/dev/null 2>&1
then
    echo Installing dependencies on Ubuntu...

    install_ubuntu libx11-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxi-dev libxmu-dev libblas-dev libxinerama-dev libxcursor-dev
    install_ubuntu libassimp-dev
    install_ubuntu libglfw3-dev
    install_cmake "meshoptimizer"

    exit
fi

# MacOS
if brew update >/dev/null 2>&1
then
    echo Installing dependencies on MacOS...

    install_macos glfw
    install_macos assimp
    install_cmake "meshoptimizer"

    exit
fi

deps=("assimp" "glfw" "meshoptimizer")
for (( i = 0 ; i < ${#deps[@]} ; i++ ))
do
    if ! install_cmake "${deps[$i]}"
    then
        exit 1
    fi
done
