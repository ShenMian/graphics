// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

using namespace std::literals::string_literals;

void PrintInfo();

int main()
{
	try
	{
		// Renderer::setAPI(Renderer::API::OpenGL); // 设置渲染 API 为 OpenGL
		
		Window::init();

		auto window = new Window("Example", Monitor::getPrimary().getSize() / 2);

		Input::setWindow(window);
		Renderer::init(*window);

		PrintInfo();

		Model model;
		model.load("../../../3DModel/basic/cube.obj");
		// model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj");
		// model.load("../../../3DModel/scene/Amazon_Lumberyard_Bistro/Exterior/exterior.obj");
		// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf");
		// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx"); // 暂不支持 DDS 格式的纹理资源
		/*model.loadAsync("../../../3DModel/scene/Crytek_Sponza/sponza.obj", [](std::string_view error){
			if(!error.empty())
				puts(error.data());
		});*/

		auto camera = PerspectiveCamera::create(60.f, (float)window->getSize().x / window->getSize().y, 0.1f, 500.0f);

		auto program = Program::create("Shaders/mesh");
		auto pipeline = Pipeline::create(program);

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
		window->onResize = [&](Vector2i size)
		{
			camera->setProjection(60.f, (float)size.x / size.y, 0.1f, 500.0f);
		};
		window->setVisible(true); // 设置窗口可见

		while(running)
		{
			program->setUniform("view", Matrix4::translate({0, 0, 3}));
			// program->setUniform("view", camera->getView());
			program->setUniform("projection", camera->getProjection());

			cmdBuffer->begin();
			{
				cmdBuffer->setViewport({0, 0}, window->getSize());
				cmdBuffer->setClearColor({0, 0, 0, 0});
				cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

				cmdBuffer->setPipeline(pipeline);
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
	}
	catch(std::runtime_error& e)
	{
		puts(e.what());
	}

	return 0;
}

void PrintInfo()
{
	// 打印显示器信息
	for(const auto& mon : Monitor::getMonitors())
	{
		/*printf(std::format(
			"Monitor\n"
			"|-Name        : {}\n"
			"|-Size        : {}x{}\n"
			"`-Refresh rate: {} Hz\n",
			mon.getName(), mon.getSize().x, mon.getSize().y, mon.getRefreshRate()).c_str());*/
		printf(
			"Monitor\n"
			"|-Name        : %s\n"
			"|-Size        : %dx%d\n"
			"`-Refresh rate: %d Hz\n",
			mon.getName().c_str(), mon.getSize().x, mon.getSize().y, mon.getRefreshRate());
	}

	// 打印基本信息
	const auto renderer = Renderer::get();
	/*printf(std::format(
		"Basic\n"
		"|-Device  : {}\n"
		"|-Renderer: {}\n"
		"`-Vendor  : {}\n",
		renderer->getDeviceName(), renderer->getRendererName(), renderer->getVendorName()).c_str());*/
	printf(
		"Renderer\n"
		"|-Device  : %s\n"
		"|-Renderer: %s\n"
		"`-Vendor  : %s\n",
		renderer->getDeviceName().c_str(),
		renderer->getRendererName().c_str(),
		renderer->getVendorName().c_str());
}
