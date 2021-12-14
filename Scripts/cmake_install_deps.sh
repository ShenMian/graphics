#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

function install() {
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

if apt-get >/dev/null 2>&1
then
    sudo apt-get update
    sudo apt-get install -y libx11-dev
    sudo apt-get install -y mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev
    sudo apt-get install -y libxrandr-dev
    sudo apt-get install -y libxi-dev
    sudo apt-get install -y libxmu-dev
    sudo apt-get install -y libblas-dev
    sudo apt-get install -y libxinerama-dev
    sudo apt-get install -y libxcursor-dev
    sudo apt-get install -y libassimp-dev
    sudo apt-get install -y libglfw3-dev
    install "meshoptimizer"
    exit
fi

deps=("assimp" "glfw" "meshoptimizer")
for (( i = 0 ; i < ${#deps[@]} ; i++ ))
do
    if ! install "${deps[$i]}"
    then
        exit 1
    fi
done
