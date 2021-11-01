// Copyright 2021 SMS
// License(Apache-2.0)

#include "Graphics.h"

int main()
{
	Window::init();

	Window window("Example", {1920 / 2, 1080 / 2});
	window.setVisible(true);

	Model model;
	Timer timer;
	model.loadAsync("../../3DModel/scene/SunTemple/SunTemple.fbx", [&](std::error_code ec)
	{
		if(ec) return;
		puts(std::format("模型加载完毕: {}ms", timer.getMilliseconds()).c_str());
	});

	auto renderer = Renderer::get();
	puts(std::format("Device:   {}", renderer->getDeviceName()).c_str());
	puts(std::format("Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("Vendor:   {}", renderer->getVendorName()).c_str());

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		cmdBuffer->begin();
		{
			cmdBuffer->setClearColor({0, 0.3, 0, 0});
			cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	return 0;
}