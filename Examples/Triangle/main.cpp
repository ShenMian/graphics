// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

#define FMT_HEADER_ONLY
#include <fmt/core.h>

void PrintInfo();

struct Vertex
{
	Vector2 position;
	Vector3 color;
};

int main()
{
	Renderer::setAPI(Renderer::API::OpenGL);
	Window::init();

	{
		Window window("Triangle", Monitor::getPrimary().getSize() / 2);
		Renderer::init(window);

		{
			PrintInfo();

            // 创建一个由 3 个顶点构成的三角形
			const std::vector<Vertex> vertices = {
				{{0,     0.5}, {1, 0, 0}},
				{{0.5,  -0.5}, {0, 1, 0}},
				{{-0.5, -0.5}, {0, 0, 1}}
			};
			VertexAttributes layout = {
				{"position", Format::RG32F},
				{"color",    Format::RGB32F}
			};
			layout.setStride(sizeof(Vertex));
			auto vertexBuffer = VertexBuffer::create(vertices, layout);

            // 创建着色器程序
			auto program = Program::create("Shaders/forward");

			Pipeline::Descriptor desc;
			desc.program = program;
			desc.vertexAttributes = layout;
			auto pipeline = Pipeline::create(desc);

			auto cmdQueue = CommandQueue::create();
			auto cmdBuffer = CommandBuffer::create();

			bool running = true;
			window.onClose = [&]() { running = false; };
			window.setVisible(true); // 设置窗口可见

			while(running)
			{
				cmdBuffer->begin();
				{
					cmdBuffer->setPipeline(pipeline);

					cmdBuffer->beginRenderPass();
					{
						cmdBuffer->setViewport({window.getSize()});
						cmdBuffer->setClearColor({0, 0, 0, 0});
						cmdBuffer->clear(ClearFlag::Color);

						cmdBuffer->setVertexBuffer(vertexBuffer);
						cmdBuffer->draw(vertexBuffer->getCount());
					}
					cmdBuffer->endRenderPass();
				}
				cmdBuffer->end();
				cmdQueue->submit(cmdBuffer);

				window.update();
			}
		}

		Renderer::deinit();
	}

	Window::deinit();

	return 0;
}

void PrintInfo()
{
    // 打印显示器信息
    for(const auto& mon : Monitor::getMonitors())
    {
        printf("%s", fmt::format(
                "Monitor\n"
                "|-Name        : {}\n"
                "|-Size        : {}x{}\n"
                "`-Refresh rate: {} Hz\n",
                mon.getName(), mon.getSize().x, mon.getSize().y, mon.getRefreshRate()).c_str());
    }

    // 打印基本信息
    const auto renderer = Renderer::get();
    printf("%s", fmt::format(
            "Basic\n"
            "|-Device  : {}\n"
            "|-Renderer: {}\n"
            "`-Vendor  : {}\n",
            renderer->getDeviceName(), renderer->getRendererName(), renderer->getVendorName()).c_str());
}
