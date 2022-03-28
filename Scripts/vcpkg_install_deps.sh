#!/bin/bash
# Copyright 2021 ShenMian
# License(Apache-2.0)

# 獲取管理員權限
if ! sudo echo
then
    echo "Can not get sudo permission."
    exit 1
fi

echo Installing dependencies...
sudo vcpkg install assimp glfw3 meshoptimizer freetype
