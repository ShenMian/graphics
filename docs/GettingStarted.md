# 快速入门 {#GettingStarted}

[TOC]

## 用法

### CMake

通过 CMake 可以很快速/简单的使用该库.

1. 将该仓库作为子模块加入你的仓库. 在你的仓库下执行下面的命令.

   ```bash
   git submodule add https://github.com/ShenMian/Graphics --recursive
   ```

2. 在 CMakeLists.txt 里加入下面代码. 请根据具体情况做适当修改.

   ```cmake
   add_subdirectory(Graphics)              # 构建 Graphics 并生成静态库, 参数为 Graphics 库根目录的相对路径
   add_executable(YourApp YourApp.cpp)     # 你的可执行文件, 里面包含了对 Graphics API 的调用
   target_link_libraries(YourApp graphics) # 连接 Graphics 静态库, 也可以使用 graphics::graphics
   ```

3. 若要设置 option 可以在 `add_subdirectory` 前加上下面代码:

   ```cmake
   SET(OPTION_NAME ON CACHE BOOL "" FORCE)  # 开启选项 OPTION_NAME
   SET(OPTION_NAME OFF CACHE BOOL "" FORCE) # 关闭选项 OPTION_NAME
   ```

**警告**: 请查看 [依赖项](https://github.com/ShenMian/Graphics/blob/main/Deps/README.md) 来确保不会产生菱形依赖. 或提前安装依赖项来减少产生菱形依赖的可能性, 执行 `vcpkg install` 命令或执行脚本 `Scripts/cmake_install_deps`.  
**注意**: 因为该项目包含大量 [依赖项](https://github.com/ShenMian/Graphics/blob/main/Deps/README.md), 所以可以通过执行 `vcpkg install` 命令或执行脚本 `Scripts/cmake_install_deps` 提前安装依赖项来加快编译速度.  

### Premake

通过 Premake 生成你所使用 IDE 的项目文件.  

```bash
./Scripts/gen_gmake2 # 生成 GNU Makefile
./Scripts/gen_vs2022 # 生成 Visual Studio 2022 解决方案
./Scripts/gen_xcode4 # 生成 Xcode4 项目文件
```

除此之外, 你还可以自行调用 premake5 生成你想要的内容. 详情请查看[相关文档](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files).  

该项目编译后会生成一个静态库文件, 在生成你的可执行文件时需链接该静态库.  

## 更新

因为该库对 Git 子模块的更新较为频繁, 请注意拉取后将子模块签出到相应的提交. 你可以使用下面的命令更新本地克隆仓库/子模块.  

```bash
git pull                    # 拉取到最新的提交
git checkout <tag-id>       # 切换到指定的版本
git submodule update --init # 签出子模块
```

**警告**: 不建议直接使用最新提交, 因为这些修改还不稳定, 甚至无法正常工作.  

## 应用骨架

```cpp
#include <Graphics/Graphics.h> // 引入 Graphics 头文件, 只需包含该文件

// 初始化, 需按照严格的顺序. 在使用任何 API 前调用初始化即可
Renderer::setAPI(Renderer::API::OpenGL); // 选择将要使用的渲染后端 API
                                         // 该步骤必须在初始化窗口前执行, 因为窗口实现依赖于渲染后端 API
Window::init();   // 初始化窗口, 之后才可以调用窗口/显示器/输入相关的 API

const auto monitor = Monitor::getPrimary();   // 获取主显示器
{
    Window window("title", monitor->getSize() / 2); // 创建窗口, 窗口标题为 title, 大小为主显示器分辨率的一半,
                                                    // 即窗口应该占据屏幕的 1/4
                                                    
    Renderer::init(window); // 初始化渲染 API, 之后才可以使用渲染相关的 API
    UI::init(window);       // (可选) 初始化 UI, 之后才能使用 UI 相关的 API.
                            // 如果你不打算使用 UI 相关的 API, 可以跳过此步骤

    window.setVisible(true); // 显示窗口
                                                   
    {
        // ... 构建渲染相关对象实例 ...
        
        while(true) // 主循环
        {
            UI::beginFrame();
            
            // ... UI 相关代码 ...
            // ... 渲染相关代码 ...
            
            UI::endFrame();

            window.update(); // 更新窗口, 展示结果并进行事件回调
        }
    } // 析构渲染相关实例
    
    // 清理, 顺序与初始化相反
    UI::deinit();
    Renderer::deinit(); // 调用该语句前应确保所以渲染相关的对象已被销毁
} // 析构窗口实例
Window::deinit(); // 调用该语句前应该确保所有窗口已被销毁
```

## 输入

- 键盘/鼠标输入: @ref Input.
- 手柄(控制器)输入: @ref Gamepad.

## 示例

阅读 [示例](https://github.com/ShenMian/Graphics/tree/main/Examples) 是一种快速的入门方式.  
从示例中可以很直观的看到各种常见 API 的使用方式.  
