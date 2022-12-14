#!/bin/bash
# Copyright 2022 ShenMian
# License(Apache-2.0)

function install_arch() {
    deps=$*

    echo "Installing '$deps'..."
    if ! sudo pacman --noconfirm -S $deps >/dev/null; then
        echo "pacman install failed"
        exit 1
    fi
}

function install_ubuntu() {
    deps=$*

    echo "Installing '$deps'..."
    if ! sudo apt install -y $deps >/dev/null; then
        echo "apt install failed"
        exit 1
    fi
}

function install_macos() {
    deps=$*

    echo "Installing '$deps'..."
    if ! brew install $deps >/dev/null; then
        echo "brew install failed"
        exit 1
    fi
}

function install_cmake() {
    dep=$1

    cd "$dep" || return 1

    echo "Installing '$dep'..."

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

    cd .. || return 1
}

# 切换到 Deps 目录
cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd ../deps || exit

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

# Arch Linux
if sudo pacman -Syyu >/dev/null 2>&1
then
    echo Installing dependencies on Arch Linux...

    :'
    if ! glxinfo | grep -i vulkan 2>/dev/null
    then
        install_arch vulkan-intel
        install_arch vulkan-radeon
        yay -S amdgpu-pro-vulkan
    fi
    '

    install_arch cmake
    install_cmake "meshoptimizer"

    exit
fi

# Ubuntu
if sudo apt update >/dev/null 2>&1
then
    echo Installing dependencies on Ubuntu...

    wget -qO - https://packages.lunarg.com/lunarg-signing-key-pub.asc | sudo apt-key add -
    sudo wget -qO /etc/apt/sources.list.d/lunarg-vulkan-bionic.list https://packages.lunarg.com/vulkan/lunarg-vulkan-bionic.list
    sudo apt update >/dev/null

    :'
    if ! vulkaninfo | grep -i vulkan 2>/dev/null
    then
        # AMD GPU
        sudo add-apt-repository ppa:oibaf/graphics-drivers
        sudo apt update
        sudo apt upgrade
        install_ubuntu libvulkan1 mesa-vulkan-drivers vulkan-utils

        # Nvidia GPU
        # sudo add-apt-repository ppa:graphics-drivers/ppa
        # sudo apt update
        # sudo apt upgrade
        # install_ubuntu nvidia-graphics-drivers-396 nvidia-settings vulkan vulkan-utils
    fi
    '

    install_ubuntu cmake
    install_ubuntu vulkan-headers libvulkan-dev vulkan-tools vulkan-validationlayers-dev spirv-tools
    install_ubuntu libx11-dev mesa-common-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxi-dev libxmu-dev libblas-dev libxinerama-dev libxcursor-dev xorg-dev
    install_ubuntu libglfw3-dev
    # install_ubuntu libassimp-dev
    install_cmake "assimp"
    install_cmake "meshoptimizer"

    exit
fi

# MacOS
if brew update >/dev/null 2>&1
then
    echo Installing dependencies on MacOS...

    install_macos cmake
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
