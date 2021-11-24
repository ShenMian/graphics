# Copyright 2021 ShenMian
# License(Apache-2.0)

# 切换到主目录
cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd ..

# 安装依赖项
./Scripts/cmake_install_deps.sh || exit

# 生成 CMake 緩存
echo Generating CMake cache...
cmake -B build >/dev/null
if [ $? -ne 0 ]; then
    echo Failed to generate CMake cache.
    exit 1
fi

# 構建
echo Building...
cmake --build build >/dev/null
if [ $? -ne 0 ]; then
    echo Failed to build.
    cmake --build build
    exit 1
fi

echo Done.
