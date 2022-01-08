// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

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

#if 0
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
#endif

		Model model;
		// model.load("../../../3DModel/basic/cube.obj");
		model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj");
		// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf");
		// model.load("../../../3DModel/scene/Amazon_Lumberyard_Bistro/Exterior/exterior.obj");
		// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx"); // 暂不支持 DDS 格式的纹理资源
		/*model.loadAsync("../../../3DModel/scene/Crytek_Sponza/sponza.obj", [](std::string_view error){
			if(!error.empty())
				puts(error.data());
		});*/

		auto program = Program::create("Shaders/pbr");
		auto pipeline = Pipeline::create(program);

		auto cmdQueue = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.setPerspective(radians(60.f), (float)window->getSize().x / window->getSize().y, 0.1f, 5000.f);
		camera.setPosition({0, 0, 3});

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
		window->onMouseMove = [&](Vector2d position)
		{
			static Vector2f lastPos = position;

			const auto pos = static_cast<Vector2f>(position);
			const Vector2f mouseSensitivity = Vector2f::unit * 0.07f;
			Vector2f offset = pos - lastPos;
			lastPos = pos;
			offset.x *= mouseSensitivity.x;
			offset.y *= mouseSensitivity.y;

			auto rot = camera.getRotation();
			rot.y += offset.x;
			rot.x += -offset.y;
			camera.setRotation(rot);
		};
		window->onResize = [&](Vector2i size)
		{
			camera.setPerspective(radians(60.f), (float)size.x / size.y, 0.1f, 5000.0f);
		};
		window->setVisible(true);

		window->setCursorLock(true);
		window->setRawMouseMotion(true);

		ui::Window win("Debug");
		ui::Label label0("camera");
		ui::Label label1("");
		ui::Label label2("");
		ui::Button btn("Button");
		ui::Menu menu("File");
		win.add(label0);
		win.add(label1);
		win.add(label2);
		win.add(btn);
		win.add(menu);

		while(running)
		{
			UI::begin();

			// FIXME: 向上移动时向下移动
			const float speed = 1.1f;
			if(Input::isPressed(Key::W))
				camera.setPosition(camera.getPosition() + camera.getFront() * speed);
			if(Input::isPressed(Key::S))
				camera.setPosition(camera.getPosition() - camera.getFront() * speed);
			if(Input::isPressed(Key::A))
				camera.setPosition(camera.getPosition() - camera.getRight() * speed);
			if(Input::isPressed(Key::D))
				camera.setPosition(camera.getPosition() + camera.getRight() * speed);
			if(Input::isPressed(Key::E))
				camera.setPosition(camera.getPosition() + camera.getUp() * speed);
			if(Input::isPressed(Key::Q))
				camera.setPosition(camera.getPosition() - camera.getUp() * speed);

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
