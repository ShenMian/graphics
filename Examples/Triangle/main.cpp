// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

void PrintInfo();

struct Vertex
{
	Vector2 position;
	Vector3 color;
};

int main()
{
	Renderer::setAPI(Renderer::API::Vulkan);

	Window::init();

	auto window = new Window("Triangle", Monitor::getPrimary().getSize() / 2);

	Renderer::init(*window);

	PrintInfo();

	const std::vector<Vertex> vertices = {
		{{0,     0.5}, {1, 0, 0}},
		{{0.5,  -0.5}, {0, 1, 0}},
		{{-0.5, -0.5}, {0, 0, 1}}
	};

	VertexLayout layout = {
		{"position", Format::RG32F},
		{"color",    Format::RGB32F}
	};
	layout.setStride(sizeof(Vertex));

	auto vertexBuffer = VertexBuffer::create(vertices, layout);

	Program::Descriptor programDesc;
	{
		// TODO
		if(Renderer::getAPI() == Renderer::API::OpenGL)
		{
			Shader::Descriptor vertShaderDesc;
			vertShaderDesc.stage = Shader::Stage::Vertex;
			vertShaderDesc.path = "Shaders/forward.vert.glsl";
			programDesc.vertex = Shader::create(vertShaderDesc);

			Shader::Descriptor fragShaderDesc;
			fragShaderDesc.stage = Shader::Stage::Fragment;
			fragShaderDesc.path = "Shaders/forward.frag.glsl";
			programDesc.fragment = Shader::create(fragShaderDesc);
		}
		else
		{
			Shader::Descriptor vertShaderDesc;
			vertShaderDesc.stage = Shader::Stage::Vertex;
			vertShaderDesc.path = "Shaders/forward.vert.spv";
			programDesc.vertex = Shader::create(vertShaderDesc);

			Shader::Descriptor fragShaderDesc;
			fragShaderDesc.stage = Shader::Stage::Fragment;
			fragShaderDesc.path = "Shaders/forward.frag.spv";
			programDesc.fragment = Shader::create(fragShaderDesc);
		}
	}
	auto program = Program::create(programDesc);

	auto cmdQueue = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	Pipeline::Descriptor desc;
	desc.program = program;
	desc.vertexBuffer = vertexBuffer;
	desc.viewports = {Viewport(Monitor::getPrimary().getSize())};
	auto pipeline = Pipeline::create(desc);

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
			cmdBuffer->setPipeline(pipeline);
			cmdBuffer->setViewport({window->getSize()});
			cmdBuffer->setClearColor({0, 0, 0, 0});
			cmdBuffer->clear(ClearFlag::Color);

			cmdBuffer->setVertexBuffer(vertexBuffer);
			cmdBuffer->draw(vertexBuffer->getCount());
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
