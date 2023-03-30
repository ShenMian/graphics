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
		window->set_visible(true); // 设置窗口可见

		while(running)
		{
			cmdBuffer->begin();
			{
				cmdBuffer->set_pipeline(pipeline);

				// cmdBuffer->beginRenderPass();
				{
					cmdBuffer->set_viewport({window->get_size()});
					cmdBuffer->set_clear_color({0, 0, 0, 0});
					cmdBuffer->clear(ClearFlag::Color);

					cmdBuffer->set_vertex_buffer(vertexBuffer);
					cmdBuffer->draw(vertexBuffer->get_count());
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
