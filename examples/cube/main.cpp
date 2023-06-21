// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "../base/base.hpp"

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

		auto program = Program::create("shaders/mesh");

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
		camera.set_perspective(radians(60.f), (float)window->get_size().x() / window->get_size().y(), 0.1f, 5000.f);
		camera.set_position({0, 0, 3});

		auto matrices = UniformBuffer::create(0, 3 * sizeof(Matrix4f));

		bool running     = true;
		window->onClose  = [&] { running = false; };
		window->onResize = [&](Vector2i size) {
			camera.set_perspective(camera.get_vfov(), (float)size.x() / size.y(), camera.get_near(), camera.get_far());
		};
		window->set_visible(true);

		Matrix4f  model = Matrix4f::create_rotation_x(radians(-15.f));
		Transform transform;
		transform.rotation().eular({radians(-15.f), 0.f, 0.f});

		while(running)
		{
			model *= Matrix4f::create_rotation_y(radians(0.5f));

			// 更新 UniformBuffer
			matrices->get_buffer().map();
			matrices->get_buffer().write(camera.get_view().data(), sizeof(Matrix4f));
			matrices->get_buffer().write(camera.get_projection().data(), sizeof(Matrix4f), sizeof(Matrix4f));
			matrices->get_buffer().write(model.data(), sizeof(Matrix4f), 2 * sizeof(Matrix4f));
			matrices->get_buffer().unmap();

			cmdBuffer->begin();
			{
				cmdBuffer->set_pipeline(pipeline);

				cmdBuffer->begin_render_pass();
				{
					cmdBuffer->set_viewport({window->get_size()});
					cmdBuffer->set_clear_color({0, 0, 0, 0});
					cmdBuffer->set_clear_depth(std::numeric_limits<float>::infinity());
					cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

					cmdBuffer->set_vertex_buffer(vertexBuffer);
					cmdBuffer->set_index_buffer(indexBuffer);
					cmdBuffer->draw_indexed(indexBuffer->get_count());
				}
				cmdBuffer->end_render_pass();
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
