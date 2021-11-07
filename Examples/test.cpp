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
	Model model;
	// model.load("../../3DModel/scene/Crytek_Sponza/sponza.obj");
	// model.load("../../3DModel/scene/SunTemple/SunTemple.fbx"); // 存在不支持格式的纹理资源
	// model.load("../../3DModel/weapon/m4a1/m4a1.gltf");

	Window::init();

	Window window("Example", {1920 / 2, 1080 / 2});
	window.setVisible(true);

	auto renderer = Renderer::get();
	puts(std::format("Device:   {}", renderer->getDeviceName()).c_str());
	puts(std::format("Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("Vendor:   {}", renderer->getVendorName()).c_str());

	std::vector<Vertex> vertices = {
		{{0,     0.5}, {1, 0, 0}},
		{{0.5,  -0.5}, {0, 1, 0}},
		{{-0.5, -0.5}, {0, 0, 1}}
	};

	VertexFormat fmt = {
		{"position", Format::RG32F},
		{"color", Format::RGB32F}
	};
	fmt.setStride(sizeof(Vertex));

	auto vbo = VertexBuffer::create(vertices, fmt);

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	auto forword = Program::create("Shaders/forword");

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		forword->use();
		cmdBuffer->begin();
		{
			cmdBuffer->setClearColor({0, 0.3f, 0, 0});
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