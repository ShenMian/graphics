# Triangle

该例子渲染了单个三角形.

<p align="center"><img src="screenshot.png"/></p>

代码大致可以分为一下几个步骤.

## 初始化
进行初始化来确保第三方API可以被正常使用.
```cpp
Window::init();                        // 初始化第三方库 GLFW
Window window("Triangle", {960, 540}); // 创建一个标题为 Triangle, 大小为 960x540 像素的窗口,
                                       // 并初始化第三方库 GLAD
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
```cpp
const std::vector<Vertex> vertices = {
    {{0,     0.5}, {1, 0, 0}}, // 第一个顶点, 红色
    {{0.5,  -0.5}, {0, 1, 0}}, // 第二个顶点, 绿色
    {{-0.5, -0.5}, {0, 0, 1}}  // 第三个顶点, 蓝色
};
```

创建 VertexFormat, 用于表示顶点数据格式.
```cpp
VertexFormat format = {
    {"position", Format::RG32F}, // 2D 坐标, 包含两个 float 变量.
    {"color",    Format::RGB32F} // RGB 颜色, 包含三个 float 变量.
};
format.setStride(sizeof(Vertex)); // 设置数组中每个顶点的大小.
```
创建 VertexFormat 的过程中会通过格式自动计算单个顶点的大小. 但由于内存对齐, 实际的大小不一定等于各成员大小之和.
因此最后一行将顶点大小设置为了常量`sizeof(Vertex)`, 该值会在编译时变成实际的顶点大小.

创建顶点缓冲区.
```cpp
auto vbo = VertexBuffer::create(vertices, format);
```

## 着色器
从指定的位置寻找源文件并创建着色器程序.
```cpp
auto program = Program::create("Shaders/forword");
```
这行代码会找到 [Shaders](Shaders) 文件夹下了两个源文件, 分别是 [forword.vert.glsl](Shaders/forword.vert.glsl)(顶点着色器源文件) 和 [forword.frag.glsl](Shaders/forword.frag.glsl)(片段着色器源文件). 将其编译并链接为着色器程序.

## 命令缓冲区 & 命令队列
创建命令缓冲区和命令队列.
```cpp
auto cmdBuffer = CommandBuffer::create(); // 创建命令缓冲区
auto cmdQueue  = CommandQueue::create();  // 创建命令队列
```

## 主循环
循环渲染单个三角形.
```cpp
bool running   = true;
window.onClose = [&]() { running = false; }; // 注册一个窗口关闭按钮按下的回调,
                                             // 当窗口关闭按钮被按下时 running 的值变为 false,
                                             // 主循环在执行完循环体后退出, 程序结束
while(running) // 当该变量值为 false 时主循环退出, 程序结束
{
    program->use(); // 使用着色器程序
    cmdBuffer->begin();
    {
        cmdBuffer->setClearColor({0, 0, 0, 0}); // 设置清空颜色缓冲区的默认值
        cmdBuffer->clear(ClearFlag::Color);     // 清空颜色缓冲区
        cmdBuffer->setVertexBuffer(vbo);        // 设置顶点缓冲区
        cmdBuffer->draw(0, vbo->getCount());    // 绘制顶点缓冲区中的全部数据
    }
    cmdBuffer->end();
    cmdQueue->submit(cmdBuffer); // 提交命令缓冲区给命令队列, 开始渲染操作

    window.update(); // 更新窗口事件并交换缓冲区, 该语句执行完即可从窗口看到渲染结果
}
```

**提示**: 未提及的部分将会在源文件中包含相应的注释.
