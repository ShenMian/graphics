// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

#include <thread>

using namespace std::literals::string_literals;

void PrintInfo();

void moveForward(Camera& cam, float step)
{
	cam.setPosition(cam.getPosition() + cam.getFront() * step);
}

void moveRight(Camera& cam, float step)
{
	cam.setPosition(cam.getPosition() + cam.getRight() * step);
}

void moveUp(Camera& cam, float step)
{
	cam.setPosition(cam.getPosition() + cam.getUp() * step);
}

void lookUp(Camera& cam, float step)
{
	auto rot = cam.getRotation();
	rot.x += step;
	cam.setRotation(rot);
}

void turnRight(Camera& cam, float step)
{
	auto rot = cam.getRotation();
	rot.y += step;
	cam.setRotation(rot);
}

void processGamepad(Camera& cam, Gamepad& gamepad)
{
	if(!gamepad.isConnected())
		return;

	gamepad.update();

	float speed = 1.0f;
	if(gamepad.get(Gamepad::Button::LeftThumb))
		speed *= 3;

	const Vector2f sensitivity = Vector2f::unit * 1.5f;

	const auto leftThumb = gamepad.get(Gamepad::Thumb::left);
	const auto rightThumb = gamepad.get(Gamepad::Thumb::right);
	const auto rightTrigger = gamepad.get(Gamepad::Trigger::right);
	const auto leftTrigger = gamepad.get(Gamepad::Trigger::left);

	moveForward(cam, -leftThumb.y * speed);
	moveRight(cam, leftThumb.x * speed);

	lookUp(cam, -rightThumb.y * sensitivity.x);
	turnRight(cam, rightThumb.x * sensitivity.y);

	moveUp(cam, rightTrigger + (-leftTrigger));
}

void processKeyboard(Camera& cam)
{
	float speed = 1.0f;
	if(Input::isPressed(Key::LeftShift))
		speed *= 3;

	if(Input::isPressed(Key::W))
		moveForward(cam, speed);
	if(Input::isPressed(Key::S))
		moveForward(cam, -speed);
	if(Input::isPressed(Key::A))
		moveRight(cam, -speed);
	if(Input::isPressed(Key::D))
		moveRight(cam, speed);
	if(Input::isPressed(Key::E))
		moveUp(cam, speed);
	if(Input::isPressed(Key::Q))
		moveUp(cam, -speed);
}

void processMouse(Camera& cam)
{
	const Vector2f sensitivity = Vector2f::unit * 0.07f;

	const auto position = Input::getMousePosition();
	static Vector2f lastPos = position;
	const auto pos = static_cast<Vector2f>(position);
	Vector2f offset = pos - lastPos;
	lastPos = pos;
	offset.x *= sensitivity.x;
	offset.y *= sensitivity.y;

	lookUp(cam, -offset.y);
	turnRight(cam, offset.x);
}

void processInput(Camera& cam, Gamepad& gamepad)
{
	processGamepad(cam, gamepad);
	processKeyboard(cam);
	processMouse(cam);
}

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
		// model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj", Model::Process::Fast);
		// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf", Model::Process::Fast);
		model.load("../../../3DModel/scene/Amazon_Lumberyard_Bistro/Exterior/exterior.obj", Model::Process::Fast);
		// model.load("../../../3DModel/scene/San_Miguel/san-miguel-low-poly.obj", Model::Process::Fast);
		// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx", Model::Process::Fast); // 暂不支持 DDS 格式的纹理资源

		auto program = Program::create("Shaders/pbr");
		PipelineLayout layout = {
			{"albedo", PipelineLayout::Type::Texture, 0, PipelineLayout::StageFlags::Fragment}
		};
		auto pipeline = Pipeline::create({layout, program});

		auto cmdQueue = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.setPerspective(radians(60.f), (float)window->getSize().x / window->getSize().y, 0.1f, 5000.f);

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
					model.compress();
					break;

				case Key::O:
					model.decompress();
					break;
				}
			}
		};
		window->onScroll = [&](Vector2d offset)
		{
			static float fov = 60;
			if(1.f <= fov && fov <= 60.f)
				fov -= (float)offset.y * 3 * (fov / 60);
			fov = std::clamp(fov, 1.f, 60.f);
			camera.setPerspective(radians(fov), (float)window->getSize().x / window->getSize().y, 0.03f, 10000.f);
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

		Gamepad gamepad(0);

		while(running)
		{
			UI::begin();

			processInput(camera, gamepad);

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
