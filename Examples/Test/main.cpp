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

	Window window("Example", {1920 / 2, 1080 / 2});
	window.setVisible(true);

	auto renderer = Renderer::get();
	puts(std::format("Device:   {}", renderer->getDeviceName()).c_str());
	puts(std::format("Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("Vendor:   {}", renderer->getVendorName()).c_str());

	auto forword = Program::create("Shaders/forword");

	Model model;
	model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj");
	// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx"); // 存在不支持格式的纹理资源
	// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf");

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

			for(const auto& mesh : model.getMeshs())
			{
				const auto vbo = mesh.getVertexBuffer();
				const auto ibo = mesh.getIndexBuffer();

				cmdBuffer->setVertexBuffer(vbo);
				cmdBuffer->setIndexBuffer(ibo);
				cmdBuffer->draw(0, vbo->getCount());
			}
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	return 0;
}