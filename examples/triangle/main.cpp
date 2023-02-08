// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "../base/base.hpp"

namespace
{

struct Vertex
{
	Vector2 position;
	Vector3 color;
};

} // namespace

class Triangle final : public Base
{
public:
	Triangle() : Base("Triangle"){};

	int main(int argc, char* argv[]) override
	{
		// 创建一个由 3 个顶点构成的三角形
		// clang-format off
		const std::vector<Vertex> vertices = {
			{{0,     0.5}, {1, 0, 0}},
			{{0.5,  -0.5}, {0, 1, 0}},
			{{-0.5, -0.5}, {0, 0, 1}}
		};
		// clang-format on
		VertexFormat format       = {{"position", Format::RG32F}, {"color", Format::RGB32F}};
		auto         vertexBuffer = VertexBuffer::create(vertices, format);

		// 创建着色器程序
		auto program = Program::create("shaders/forward");

		Pipeline::Descriptor desc;
		desc.program          = program;
		desc.vertexFormat     = format;
		desc.depth.enableTest = false;
		auto pipeline         = Pipeline::create(desc);

		auto cmdQueue  = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		bool running    = true;
		window->onClose = [&] { running = false; };
		window->setVisible(true); // 设置窗口可见

		while(running)
		{
			cmdBuffer->begin();
			{
				cmdBuffer->setPipeline(pipeline);

				// cmdBuffer->beginRenderPass();
				{
					cmdBuffer->setViewport({window->getSize()});
					cmdBuffer->setClearColor({0, 0, 0, 0});
					cmdBuffer->clear(ClearFlag::Color);

					cmdBuffer->setVertexBuffer(vertexBuffer);
					cmdBuffer->draw(vertexBuffer->getCount());
				}
				// cmdBuffer->endRenderPass();
			}
			cmdBuffer->end();
			cmdQueue->submit(cmdBuffer);

			window->update();
		}

		return 0;
	}
};

int main(int argc, char* argv[])
{
	Triangle instance;
	return instance.run(argc, argv);
}
