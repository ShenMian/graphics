// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "../Base/Base.hpp"

namespace
{

struct Vertex
{
	Vector3 position;
	Vector3 color;
};

} // namespace

class Cube final : public Base
{
public:
	Cube() : Base("Cube") {}

	int main(int argc, char* argv[])
	{
		// 创建一个由 8 个顶点构成的立方体
		// clang-format off
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
		// clang-format on
		VertexFormat format       = {{"position", Format::RGB32F}, {"color", Format::RGB32F}};
		auto         vertexBuffer = VertexBuffer::create(vertices, format);

		// 创建顶点索引, 表示构成 12 个三角形的顶点组成
		// clang-format off
		const std::vector<uint32_t> indices = {
			2,0,1, 2,3,0,
			4,6,5, 4,7,6,
			0,7,4, 0,3,7,
			1,0,4, 1,4,5,
			1,5,2, 5,6,2,
			3,6,7, 3,2,6
		};
		// clang-format on
		auto indexBuffer = IndexBuffer::create(indices);

		auto program = Program::create("Shaders/mesh");

		PipelineLayout layout = {
		    {"matrices", 0, PipelineLayout::Type::UniformBuffer, PipelineLayout::StageFlags::Vertex}};
		Pipeline::Descriptor desc;
		desc.layout       = layout;
		desc.program      = program;
		desc.vertexFormat = format;
		auto pipeline     = Pipeline::create(desc);

		auto cmdQueue  = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.setPerspective(radians(60.f), (float)window->getSize().x / window->getSize().y, 0.1f, 5000.f);
		camera.setPosition({0, 0, 3});

		auto matrices = UniformBuffer::create(0, 3 * sizeof(Matrix4f));

		bool running     = true;
		window->onClose  = [&] { running = false; };
		window->onResize = [&](Vector2i size) {
			camera.setPerspective(camera.getVFOV(), (float)size.x / size.y, camera.getNear(), camera.getFar());
		};
		window->setVisible(true);

		Matrix4f  model = Matrix4f::createRotationX(radians(-15.f));
		Transform transform;

		while(running)
		{
			model *= Matrix4f::createRotationY(radians(0.5f));

			// 更新 UniformBuffer
			matrices->getBuffer().map();
			matrices->getBuffer().write(camera.getView().data(), sizeof(Matrix4f));
			matrices->getBuffer().write(camera.getProjection().data(), sizeof(Matrix4f), sizeof(Matrix4f));
			matrices->getBuffer().write(model.data(), sizeof(Matrix4f), 2 * sizeof(Matrix4f));
			matrices->getBuffer().unmap();

			cmdBuffer->begin();
			{
				cmdBuffer->setPipeline(pipeline);

				cmdBuffer->beginRenderPass();
				{
					cmdBuffer->setViewport({window->getSize()});
					cmdBuffer->setClearColor({0, 0, 0, 0});
					cmdBuffer->setClearDepth(std::numeric_limits<float>::infinity());
					cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

					cmdBuffer->setVertexBuffer(vertexBuffer);
					cmdBuffer->setIndexBuffer(indexBuffer);
					cmdBuffer->drawIndexed(indexBuffer->getCount());
				}
				cmdBuffer->endRenderPass();
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
	Cube instance;
	return instance.run(argc, argv);
}
