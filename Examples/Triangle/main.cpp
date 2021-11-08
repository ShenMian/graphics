// Copyright 2021 SMS
// License(Apache-2.0)

#include "Graphics.h"

struct Vertex
{
	Vector2 position;
	Vector3 color;
};

int main()
{
	Window::init(); // 初始化窗口

	Window window("Triangle", {960, 540}); // 创建一个标题为 Triangle, 大小为 960x540 像素的窗口 (并初始化 glad)
	window.setVisible(true);               // 设置窗口可见

	// 打印基本信息
	const auto renderer = Renderer::get();
	puts(std::format("Device:   {}", renderer->getDeviceName()).c_str());
	puts(std::format("Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("Vendor:   {}", renderer->getVendorName()).c_str());

	// 创建顶点数据, 一个有三种颜色的三角形
	const std::vector<Vertex> vertices = {
		{{0,     0.5}, {1, 0, 0}},
		{{0.5,  -0.5}, {0, 1, 0}},
		{{-0.5, -0.5}, {0, 0, 1}}
	};

	// 创建顶点数据格式
	VertexFormat format = {
		{"position", Format::RG32F},
		{"color", Format::RGB32F}
	};
	format.setStride(sizeof(Vertex)); // 设置每个顶点数据的大小

	auto vbo = VertexBuffer::create(vertices, format); // 创建顶点缓冲区

	auto forword = Program::create("Shaders/forword"); // 从源文件创建着色器(Shader)程序

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		forword->use();
		cmdBuffer->begin();
		{
			cmdBuffer->setClearColor({0, 0, 0, 0});
			cmdBuffer->clear(ClearFlag::Color);
			cmdBuffer->setVertexBuffer(vbo);
			cmdBuffer->draw(0, vbo->getCount());
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	return 0;
}