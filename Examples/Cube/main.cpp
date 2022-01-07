// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

void PrintInfo();

struct Vertex
{
	Vector3 position;
	Vector3 color;
};

int main()
{
	Renderer::setAPI(Renderer::API::OpenGL);

	Window::init();

	auto window = new Window("Cube", Monitor::getPrimary().getSize() / 2);

	Input::setWindow(window);
	Renderer::init(*window);

	PrintInfo();

	const std::vector<Vertex> vertices = {
		{{-0.5, -0.5, -0.5}, {1, 0, 0}},
		{{-0.5, +0.5, -0.5}, {0, 1, 0}},
		{{+0.5, +0.5, -0.5}, {0, 0, 1}},
		{{+0.5, -0.5, -0.5}, {1, 1, 1}},
		{{-0.5, -0.5, +0.5}, {1, 1, 0}},
		{{-0.5, +0.5, +0.5}, {0, 1, 1}},
		{{+0.5, +0.5, +0.5}, {1, 0, 1}},
		{{+0.5, -0.5, +0.5}, {0.2f, 0.2f, 0.2f}}
	};

	VertexFormat format = {
		{"position", Format::RGB32F},
		{"color", Format::RGB32F}
	};
	format.setStride(sizeof(Vertex));

	auto vertexBuffer = VertexBuffer::create(vertices, format);

	const std::vector<uint32_t> indices = {
		2,0,1, 2,3,0,
		4,6,5, 4,7,6,
		0,7,4, 0,3,7,
		1,0,4, 1,4,5,
		1,5,2, 5,6,2,
		3,6,7, 3,2,6
	};

	auto indexBuffer = IndexBuffer::create(indices);

	auto program = Program::create("Shaders/mesh");
	auto pipeline = Pipeline::create(program);

	auto cmdQueue = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	auto camera = PerspectiveCamera::create(radians(60.f), (float)window->getSize().x / window->getSize().y, 0.1f, 5000.f);
	camera->setPosition({0, 0, 3});

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
		camera->setProjection(radians(60.f), (float)size.x / size.y, 0.1f, 5000.0f);
	};
	window->setVisible(true);

	Matrix4f model = Matrix4f::createRotationX(radians(15.f));

	while(running)
	{
		model *= Matrix4f::createRotationY(radians(0.5f));

		program->setUniform("model", model);
		program->setUniform("view", camera->getView());
		program->setUniform("projection", camera->getProjection());

		cmdBuffer->begin();
		{
			cmdBuffer->setViewport({0, 0}, window->getSize());
			cmdBuffer->setClearColor({0, 0, 0, 0});
			cmdBuffer->setClearDepth(std::numeric_limits<float>::infinity());
			cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

			cmdBuffer->setVertexBuffer(vertexBuffer);
			cmdBuffer->setIndexBuffer(indexBuffer);
			cmdBuffer->drawIndexed(0, indexBuffer->getCount());
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window->update();
	}
	delete window;

	Renderer::deinit();
	Window::deinit();

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
