// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "OpenGL/GLUniformBuffer.h"

void PrintMonitorInfo();
void PrintRendererInfo();

struct Vertex
{
	Vector3 position;
	Vector3 color;
};

int main()
{
	Renderer::setAPI(Renderer::API::OpenGL);
	Window::init();

	try
	{
		Window window("Cube", Monitor::getPrimary()->getSize() / 2);
		Renderer::init(window);

		{
			PrintMonitorInfo();
			PrintRendererInfo();

            // 创建一个由 8 个顶点构成的立方体
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
			VertexAttributes format = {
				{"position", Format::RGB32F},
				{"color", Format::RGB32F}
			};
			format.setStride(sizeof(Vertex));
			auto vertexBuffer = VertexBuffer::create(vertices, format);

            // 创建顶点索引, 表示构成 12 个三角形的顶点组成
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

			auto cmdQueue = CommandQueue::create();
			auto cmdBuffer = CommandBuffer::create();

			Camera camera(Camera::Type::Perspective);
			camera.setPerspective(radians(60.f), (float)window.getSize().x / window.getSize().y, 0.1f, 5000.f);

			GLUniformBuffer matrices(0, 3 * sizeof(Matrix4f));

			bool running = true;
			window.onClose = [&]{ running = false; };
			window.onResize = [&](Vector2i size)
			{
				camera.setPerspective(camera.getVFOV(), (float)size.x / size.y, camera.getNear(), camera.getFar());
			};
			window.setVisible(true);

			Matrix4f model = Matrix4f::createRotationX(radians(-15.f));

			while(running)
			{
				model *= Matrix4f::createRotationY(radians(0.5f));

				// 更新 UniformBuffer
				matrices.getBuffer().map();
				matrices.getBuffer().write(camera.getView().data(), sizeof(Matrix4f));
				matrices.getBuffer().write(camera.getProjection().data(), sizeof(Matrix4f), sizeof(Matrix4f));
				matrices.getBuffer().write(model.data(), sizeof(Matrix4f), 2 * sizeof(Matrix4f));
				matrices.getBuffer().unmap();

				cmdBuffer->begin();
				{
					cmdBuffer->setViewport({window.getSize()});
					cmdBuffer->setClearColor({0, 0, 0, 0});
					cmdBuffer->setClearDepth(std::numeric_limits<float>::infinity());
					cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

					cmdBuffer->setVertexBuffer(vertexBuffer);
					cmdBuffer->setIndexBuffer(indexBuffer);
					cmdBuffer->drawIndexed(indexBuffer->getCount());
				}
				cmdBuffer->end();
				cmdQueue->submit(cmdBuffer);

				window.update();
			}
		}

		Renderer::deinit();
	}
	catch(const std::runtime_error& e)
	{
		puts(e.what());
	}

	Window::deinit();

	return 0;
}

void PrintMonitorInfo()
{
	printf("Monitor\n");
	for(const auto& mon : Monitor::getMonitors())
	{
		printf("%s", fmt::format(
			"|-{}\n"
			"  |-Size        : {}x{}\n"
			"  `-Refresh rate: {} Hz\n",
			mon.getName(), mon.getSize().x, mon.getSize().y, mon.getRefreshRate()).c_str());
	}
}

void PrintRendererInfo()
{
	const auto renderer = Renderer::get();
	printf("%s", fmt::format(
		"Renderer\n"
		"|-Device  : {}\n"
		"|-Renderer: {}\n"
		"`-Vendor  : {}\n",
		renderer->getDeviceName(), renderer->getRendererName(), renderer->getVendorName()).c_str());
}
