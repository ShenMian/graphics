// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

#include <thread>

using namespace std::literals::string_literals;

void PrintInfo();

int main()
{
	try
	{
		Renderer::setAPI(Renderer::API::OpenGL);

		Window::init();

		auto window = new Window("Test", Monitor::getPrimary().getSize() / 2);

		Input::setWindow(window);
		Renderer::init(*window);
		UI::init(*window);

		PrintInfo();

		/*
		const std::vector<Image> images = {
			{"../../../3DModel/skybox/cube/right.jpg"},
			{"../../../3DModel/skybox/cube/left.jpg"},
			{"../../../3DModel/skybox/cube/top.jpg"},
			{"../../../3DModel/skybox/cube/bottom.jpg"},
			{"../../../3DModel/skybox/cube/front.jpg"},
			{"../../../3DModel/skybox/cube/back.jpg"}
		};
		auto cubemap = Texture::create(images);
		auto program = Program::create("Shaders/skybox");
		cubemap->bind(0);
		program->setUniform("cubemap", 0);
		*/

		Model model;
		// model.load("../../../3DModel/basic/cube.obj");
		// model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj", Model::Process::Fast);
		model.load("../../../3DModel/weapon/m4a1/m4a1.gltf", Model::Process::Fast);
		// model.load("../../../3DModel/scene/Amazon_Lumberyard_Bistro/Exterior/exterior.obj", Model::Process::Fast);
		// model.load("../../../3DModel/scene/San_Miguel/san-miguel-low-poly.obj", Model::Process::Fast);
		// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx", Model::Process::Fast); // 暂不支持 DDS 格式的纹理资源

		auto program = Program::create("Shaders/pbr");
		PipelineLayout layout = {
			{"albedo", PipelineLayout::Type::Texture, 0, PipelineLayout::StageFlags::Fragment}
		};
		Pipeline::Descriptor desc;
		desc.layout = layout;
		desc.program = program;
		auto pipeline = Pipeline::create(desc);

		auto cmdQueue = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.setPerspective(radians(45.f), (float)window->getSize().x / window->getSize().y, 0.1f, 5000.f);

		Controller controller;
		controller.setCamera(camera);
		Gamepad gamepad(0);
		controller.setGamepad(gamepad);

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

				case Key::P:
					window->setCursorLock(false);
					break;
				}
			}
		};
		window->onScroll = [&](Vector2d offset)
		{
			static float fov = camera.getVFOV();
			if(1.f <= fov && fov <= 60.f)
				fov -= (float)offset.y * 3 * (fov / 60);
			fov = std::clamp(fov, 1.f, 60.f);
			camera.setPerspective(radians(fov), (float)window->getSize().x / window->getSize().y, 0.03f, 10000.f);
		};
		window->onResize = [&](Vector2i size)
		{
			camera.setPerspective(radians(camera.getVFOV()), (float)size.x / size.y, 0.1f, 5000.0f);
		};
		window->setVisible(true);

		window->setCursorLock(true);
		window->setRawMouseMotion(true);

		ui::Window win("Debug");
		ui::Label label0("camera");
		ui::Label label1("");
		ui::Label label2("");
		ui::Button compress("Compress");
		compress.on = [&](ui::Button& btn) {
			static bool compressed = false;
			if(!compressed)
			{
				model.compress();
				btn.setLabel("Decompress");
			}
			else
			{
				model.decompress();
				btn.setLabel("Compress");
			}
		};

		ui::Menu menu("File");
		win.add(label0);
		win.add(label1);
		win.add(label2);
		win.add(compress);
		win.add(menu);

		Timer timer;
		while(running)
		{
			const float dt = timer.getSeconds();
			controller.update(dt);
			timer.restart();

			UI::begin();

			const auto pos = camera.getPosition();
			const auto dir = camera.getRotation();
			label1.setText("  position: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
			label2.setText("  rotation: " + std::to_string(dir.x) + ", " + std::to_string(dir.y) + ", " + std::to_string(dir.z));
			win.update();

			program->setUniform("model", Matrix4f());
			program->setUniform("view", camera.getView());
			program->setUniform("projection", camera.getProjection());

			cmdBuffer->begin();
			{
				cmdBuffer->setViewport({0, 0}, window->getSize());
				cmdBuffer->setClearColor({0, 0, 0, 0});
				cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

				cmdBuffer->setPipeline(pipeline);
				for(const auto& mesh : model.getMeshs())
				{
					const auto vb = mesh.getVertexBuffer();
					const auto ib = mesh.getIndexBuffer();

					cmdBuffer->setVertexBuffer(vb);
					cmdBuffer->setIndexBuffer(ib);
					cmdBuffer->drawIndexed(ib->getCount());
				}
			}
			cmdBuffer->end();
			cmdQueue->submit(cmdBuffer);

			UI::end();

			window->update();
		}
		delete window;

		UI::deinit();
		Renderer::deinit();
		Window::deinit();
	}
	catch(std::runtime_error& e)
	{
		puts(e.what());
		exit(1);
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
