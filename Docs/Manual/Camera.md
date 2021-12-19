# 摄像机(Camera)

是观察虚拟世界的眼睛.

## 例子
```cpp
Camera camera;
camera.setType(Camera::Type::Perspective);                                          // 设置投影类型为透视投影
camera.setPerspective(60, window->getSize().x / window->getSize().y, 0.1f, 500.0f); // 设置透视投影参数
```
