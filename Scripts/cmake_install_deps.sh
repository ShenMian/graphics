
# 切换到 ThirdParty 目录
cd "$( cd "$( dirname "$0"  )" && pwd  )" || exit
cd ../ThirdParty

# 獲取管理員權限
sudo echo

# 签出第三方库
echo Checkout third-party libraries...
git submodule update --init >/dev/null
if [ $? -ne 0 ]; then
    echo Failed to checkout third-party libraries.
    exit 1
fi

echo Installing dependencies...

deps=("assimp" "glfw" "meshoptimizer")
for (( i = 0 ; i < ${#deps[@]} ; i++ ))
do
    cd ${deps[$i]}

    # 生成 CMake 緩存
    echo "|-Gerenating CMake cache..."
    cmake -B build >/dev/null
    if [ $? -ne 0 ]; then
        echo "|-Failed to generate CMake cache."
        exit 1
    fi

    # 构建
    echo "|-Building..."
    cmake --build build --config Release >/dev/null
    if [ $? -ne 0 ]; then
        echo "|-Failed to build."
        exit 1
    fi

    # 安装
    echo  "|-Installing..."
    sudo cmake --install build >/dev/null
    if [ $? -ne 0 ]; then
        echo "|-Failed to install."
        exit 1
    fi

    echo "|-Done."

    cd ..
done

