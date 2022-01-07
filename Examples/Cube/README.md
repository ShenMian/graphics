# Cube

渲染一个旋转的立方体.

<p align="center"><img src="Assets/screenshot.gif"/></p>

## 你将了解到
- 3D 模型的旋转变换.
- 绘制 3D 图像.

## 创建模型的变换矩阵
创建一个 4x4 矩阵, 用于表示改模型的变换. 包括: 位置, 旋转和缩放.
```cpp
Matrix4f model = Matrix4f::createRotationX(radians(15.f)); // 绕 X 轴旋转 15 度
```
让立方体绕 X 轴旋转 15 度可以便于我们观察的其中三个面, 更直观的感受到其在旋转.

## 旋转立方体
在主循环中更新模型的变换矩阵, 使其每次绕 Y 轴旋转 0.5 度.
```cpp
model *= Matrix4f::createRotationY(radians(0.5f));
```
由于没有对主循环的频率进行控制, 因此在性能更强劲的电脑上立方体会旋转得更快.

## 着色器
```cpp
layout (location = 0) in vec3 position; // 顶点的 3D 坐标

// ... skip ...

uniform mat4 model; // 用于接收模型的变换矩阵

void main()
{
    // ... skip ...

    // 将每个顶点坐标都乘以模型变换矩阵, 以得到变换后的顶点坐标.
    gl_Position = model * vec4(position, 1.0);
}
```

## 清空深度缓冲区
绘制 3D 图像时会使用深度缓冲区来存储每个像素的深度值, 因此在每次绘制后需要清空.
```cpp
cmdBuffer->setClearDepth(std::numeric_limits<float>::infinity()); // 设置清空深度缓冲区的默认值为无穷大
cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);            // 清空颜色缓冲区和深度缓冲区
```
