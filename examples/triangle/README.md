# Triangle

渲染单个三角形.  

<p align="center"><img src="Assets/screenshot.png"/></p>

## 你将了解到

- 窗口的创建.
- 图形渲染管线的创建.
- 绘制 2D 图像.

代码大致可以分为一下几个步骤.  

## 初始化

进行初始化来确保第三方API可以被正常使用.  

```cpp
// 设置渲染 API 为 OpenGL. 该步骤必须在初始化窗口前调用
Renderer::setAPI(Renderer::API::OpenGL);

Window::init();                        // 初始化窗口模块
Window window("Triangle", {960, 540}); // 创建一个标题为 Triangle, 大小为 960x540 像素的窗口

Renderer::init(*window);               // 初始化渲染器
```

**注意**: 若第三方库 GLAD 没有被初始化, 调用 OpenGL API 时将发生致命错误.  

## 顶点缓冲区

定义一个结构体来存储顶点数据, 包含 2D 的坐标和 RBG 颜色(0.0 - 1.0).  

```cpp
struct Vertex
{
    Vector2 position; // 2D 坐标
    Vector3 color;    // RGB 颜色
};
```

创建三个顶点, 坐标原点在视口的中心.  

![坐标系](Assets/coordinates.png)  

```cpp
const std::vector<Vertex> vertices = {
    {{0,     0.5}, {1, 0, 0}}, // 第一个顶点, 位于中上方, 红色
    {{0.5,  -0.5}, {0, 1, 0}}, // 第二个顶点, 位于右下方, 绿色
    {{-0.5, -0.5}, {0, 0, 1}}  // 第三个顶点, 位于左下方, 蓝色
};
```

创建 VertexFormat, 用于表示顶点数据格式.  

```cpp
VertexFormat format = {
    {"position", Format::RG32F}, // 2D 坐标, 包含两个 32 位的 float 变量.
    {"color",    Format::RGB32F} // RGB 颜色, 包含三个 32 位的 float 变量.
};
```

假设存放顶点数据的数组没有任何填充(padding), 创建 VertexFormat 的过程中会通过格式自动推导成员的偏移量/单个顶点的大小.  
但由于内存对齐, 实际的大小不一定等于各成员大小之和. 若无法确定结构体对齐方式, 可以使用下面方法:  

```cpp
VertexFormat format;
format.addAttribute({"position", Format::RG32F,  offsetof(Vertex, position)});
format.addAttribute({"color",    Format::RGB32F, offsetof(Vertex, color)});
format.setStride(sizeof(Vertex)); // 设置数组中每个顶点的大小.
```

创建顶点缓冲区.  

```cpp
auto vertexBuffer = VertexBuffer::create(vertices, format);
```

## 着色器

从指定的位置寻找着色器源码或 SPIR-V 文件并创建着色器程序.  

```cpp
auto program = Program::create("Shaders/forward");
```

这行代码会找到 Shaders 文件夹下了两个源文件, 分别是 [forward.vert.glsl](Shaders/forward.vert.glsl)(顶点着色器源文件) 和 [forward.frag.glsl](Shaders/forward.frag.glsl)(片段着色器源文件).  
将其编译为 SPIR-V 并载入链接为着色器程序.  

## 渲染管线

```cpp
Pipeline::Descriptor desc;
desc.program      = program; // 指定着色器程序
desc.vertexFormat = format;  // 指定顶点数据格式
auto pipeline = Pipeline::create(desc); // 创建渲染管线
```

## 命令缓冲区 & 命令队列

创建命令队列和命令缓冲区, 顺序必须固定.  

```cpp
auto cmdQueue  = CommandQueue::create();  // 创建命令队列
auto cmdBuffer = CommandBuffer::create(); // 创建命令缓冲区
```

## 主循环

循环渲染单个三角形.  

```cpp
bool running   = true; // 当该变量值为 false 时主循环退出, 程序结束
window.onClose = [&]{ running = false; }; // 注册一个窗口关闭按钮按下的回调,
                                          // 当窗口关闭按钮被按下时 running 的值变为 false,
                                          // 主循环在执行完循环体后退出, 程序结束
while(running)
{
    program->use(); // 使用着色器程序
    cmdBuffer->begin();
    {
        cmdBuffer->setPipeline(pipeline); // 设置要使用的渲染管线

        cmdBuffer->setViewport({window->getSize()}); // 设置视口
        cmdBuffer->setClearColor({0, 0, 0, 0});      // 设置清空颜色缓冲区的默认值为黑色
        cmdBuffer->clear(ClearFlag::Color);          // 清空颜色缓冲区

        cmdBuffer->setVertexBuffer(vertexBuffer);     // 设置顶点缓冲区
        cmdBuffer->draw(0, vertexBuffer->getCount()); // 绘制顶点缓冲区中的全部数据
    }
    cmdBuffer->end();
    cmdQueue->submit(cmdBuffer); // 提交命令缓冲区给命令队列, 开始渲染操作

    window.update(); // 更新窗口事件并交换缓冲区, 该语句执行完即可从窗口看到渲染结果
}
```
