// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

void PrintInfo();

int main()
{
	Window::init();

	auto window = new Window("Example", Monitor::getPrimary().getSize() / 2);
	Input::setWindow(window);

	Renderer::setAPI(Renderer::API::OpenGL); // 设置渲染 API 为 OpenGL

	PrintInfo();

	Model model;
	model.load("../../../3DModel/basic/cube.obj");
	// model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj");
	// model.load("../../../3DModel/scene/Amazon_Lumberyard_Bistro/Exterior/exterior.obj");
	// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf");
	// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx"); // 暂不支持格式的压缩纹理资源
	/*model.loadAsync("../../../3DModel/scene/Crytek_Sponza/sponza.obj", [](std::string_view error){
		if(!error.empty())
			puts(error.data());
	});*/

	auto program = Program::create("Shaders/forword");
	auto cmdQueue = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running = true;
	window->onClose = [&]() { running = false; };
	window->onKey = [&](int action, Key key)
	{
		if(action == 1)
		{
			switch(key)
			{
			case Key::Escape:
				running = false;
				break;

			case Key::F11:
				window->setFullscreen(!window->isFullscreen());
				break;
			}
		}
	};
	window->setVisible(true); // 设置窗口可见

	while(running)
	{
		program->use();
		cmdBuffer->begin();
		{
			cmdBuffer->setViewport({0, 0}, window->getSize());
			cmdBuffer->setClearColor({0, 0, 0, 0});
			cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

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

		window->update();
	}
	delete window;

	Window::deinit();

	return 0;
}

void PrintInfo()
{
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
}
