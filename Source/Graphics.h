// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Renderer.h"
#include "Pipeline.h"
#include "CommandBuffer.h"
#include "CommandQueue.h"
#include "Texture.h"
#include "VertexFormat.h"

#include "Program.h"
#include "Shader.h"

#include "Model.h"
#include "Camera/Camera.h"
#include "Camera/PerspectiveCamera.h"
#include "Camera/OrthographicCamera.h"

#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Gamepad.h"
#include "Core/Monitor.h"
#include "Core/Image.h"
#include "Core/Timer.hpp"
#include "Core/RID.hpp"

/**
 * @defgroup core 核心
 * @brief 較爲基礎的部分, 包括: 圖像導入/導出/處理, 輸入/輸出等.
 *
 * @defgroup model 模型
 * @brief 3D 模型相關. 包括: 模型導入/導出, 網格處理, 紋理材質組等.
 *
 * @defgroup shader 着色器
 * @brief 着色器相關, 包括: 着色器程序, 着色器階段等.
 */

/**
 * @mainpage Graphics 文檔
 *
 * Graphics
 * ========
 *
 * 概述
 * ----
 * 一个简单的现代图形 API 抽象层.
 *
 * 文档
 * ----
 * - @subpage GettingStarted
 */

/**
 * @page GettingStarted 快速入门
 *
 * 用法
 * ----
 * 通过 CMake 可以很快速/简单的使用该库.
 *
 * 1. 将该仓库作为子模块加入你的仓库. 在你的仓库下执行下面的命令.
 * ```bash
 * git submodule add https://github.com/ShenMian/Graphics --recursive
 * ```
 *
 * 2. 在 CMakeLists.txt 里加入下面代码. 请根据具体情况做适当修改.
 * ```cmake
 * add_subdirectory(Graphics)              # 构建 Graphics 并生成静态库
 * add_executable(YourApp YourApp.cpp)     # 你的可执行文件, 里面包含了对 Graphics API 的调用
 * target_link_libraries(YourApp graphics) # 连接 Graphics 静态库, 也可以使用 graphics::graphics
 * ```
 *
 * 通过 Premake 生成你所使用 IDE 的项目文件.
 * ```bash
 * ./Scripts/gen_gmake2 # 生成 GNU Makefile
 * ./Scripts/gen_vs2022 # 生成 Visual Studio 2022 解决方案
 * ./Scripts/gen_xcode4 # 生成 Xcode4 项目文件
 * ```
 * 除此之外, 你还可以自行调用 premake5 生成你想要的内容. 详情请查看[相关文档](https://github.com/premake/premake-core/wiki/Using-Premake#using-premake-to-generate-project-files).
 *
 * 更新
 * ---
 * 因为该库对 Git 子模块的更新较为频繁, 请注意拉取后将子模块签出到相应的提交. 你可以使用下面的命令更新本地克隆仓库/子模块.
 * ```bash
 * git pull                    # 拉取到最新的提交
 * git checkout <tag-id>       # 切换到指定的版本
 * git submodule update --init # 签出子模块
 * ```
 * **警告**: 不建议直接使用最新提交, 因为这些修改还不稳定, 可能无法正常工作.
 *
 * 示例
 * ----
 * - [Triangle](https://github.com/ShenMian/Graphics/tree/main/Examples/Triangle)
 */
