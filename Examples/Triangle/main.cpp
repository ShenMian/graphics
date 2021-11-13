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
	Window::init();
	Monitor::init();

	Window window("Triangle", Monitor::getPrimary().getSize() / 2);
	window.setVisible(true); // 设置窗口可见

	Renderer::setAPI(Renderer::API::OpenGL); // 设置渲染 API 为 OpenGL

	// 打印显示器信息
	for(const auto& mon : Monitor::getMonitors())
	{
		puts("Monitor");
		puts(std::format("|-Name        : {}", mon.getName()).c_str());
		puts(std::format("|-Size        : {}x{}", mon.getSize().x, mon.getSize().y).c_str());
		puts(std::format("`-Refresh rate: {} Hz", mon.getRefreshRate()).c_str());
	}

	// 打印基本信息
	const auto renderer = Renderer::get();
	puts("Basic");
	puts(std::format("|-Device  : {}", renderer->getDeviceName()).c_str());
	puts(std::format("|-Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("`-Vendor  : {}", renderer->getVendorName()).c_str());

	const std::vector<Vertex> vertices = {
		{{0,     0.5}, {1, 0, 0}},
		{{0.5,  -0.5}, {0, 1, 0}},
		{{-0.5, -0.5}, {0, 0, 1}}
	};

	VertexFormat format = {
		{"position", Format::RG32F},
		{"color", Format::RGB32F}
	};
	format.setStride(sizeof(Vertex));

	auto vbo = VertexBuffer::create(vertices, format);

	auto program = Program::create("Shaders/forword");

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		program->use();
		cmdBuffer->begin();
		{
			cmdBuffer->setViewport({0, 0}, window.getSize());
			cmdBuffer->setClearColor({0, 0, 0, 0});
			cmdBuffer->clear(ClearFlag::Color);
			cmdBuffer->setVertexBuffer(vbo);
			cmdBuffer->draw(0, vbo->getCount());
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	Monitor::deinit();
	Window::deinit();

	return 0;
}