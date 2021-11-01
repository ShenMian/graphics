// Copyright 2021 SMS
// License(Apache-2.0)

#include "Graphics.h"

int main()
{
	Window::init();

	Window window("Example", {1920 / 2, 1080 / 2});
	window.setVisible(true);

	auto renderer = Renderer::get();
	puts(std::format("Device:   {}", renderer->getDeviceName()).c_str());
	puts(std::format("Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("Vendor:   {}", renderer->getVendorName()).c_str());

	Model model;
	model.load("../../3DModel/scene/SunTemple/SunTemple.fbx");
	// model.load("../../3DModel/weapon/m4a1/m4a1.gltf");
	// model.load("../../3DModel/scene/Crytek_Sponza/sponza.obj");

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	auto forword = Program::create("Shaders/forword");

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		cmdBuffer->begin();
		{
			cmdBuffer->setClearColor({0, 0.3, 0, 0});
			cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

			forword->use();
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	return 0;
}