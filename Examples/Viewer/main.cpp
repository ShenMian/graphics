// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "../Base/Base.hpp"

class Viewer final : public Base
{
public:
	Viewer() : Base("Viewer"){};

	int main(int argc, char* argv[])
	{
		std::filesystem::path path;
		if(argc > 1)
			path = argv[1];
		else
		{
			path = "../../../../../../Model/sponza/sponza.glb";
			// path = "../../../../../../Model/m4a1/m4a1.gltf";
			// path = "../../../../../../Model/pistol/kimber_desert_warrior/scene.gltf";
			// path = "../../../../../../Model/Bistro/BistroExterior.glb";
			// path = "../../../../../../Model/San_Miguel/SanMiguel-low.glb";
		}

		AssimpImporter importer;
		auto           model = importer.load(path);
		printModelInfo(model);

		// model.meshs.clear();
		// model.meshs.push_back(Primitive::makeSphere(64, 32).value());
		// model.meshs.push_back(Primitive::makeIcoSphere(7));
		// model.meshs.push_back(Primitive::makeCapsule(15, 2, 0.5).value());
		// model.meshs.push_back(Primitive::makePlane(10, 10).value());

		auto program = Program::create("Shaders/mesh");

		PipelineLayout layout = {{"albedo", PipelineLayout::Type::Texture, 0, PipelineLayout::StageFlags::Fragment},
		                         {"roughness", PipelineLayout::Type::Texture, 1, PipelineLayout::StageFlags::Fragment},
		                         {"ao", PipelineLayout::Type::Texture, 2, PipelineLayout::StageFlags::Fragment},
		                         {"emissive", PipelineLayout::Type::Texture, 3, PipelineLayout::StageFlags::Fragment},
		                         {"normal", PipelineLayout::Type::Texture, 4, PipelineLayout::StageFlags::Fragment}};
		Pipeline::Descriptor desc;
		desc.layout   = layout;
		desc.program  = program;
		auto pipeline = Pipeline::create(desc);

		auto cmdQueue  = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.setPerspective(radians(45.f), (float)window->getSize().x / window->getSize().y, 0.1f, 5000.f);

		Gamepad gamepad(0);

		Controller controller;
		controller.setCamera(camera);
		controller.setGamepad(gamepad);

		auto matrices = UniformBuffer::create(0, 3 * sizeof(Matrix4f));

		/*
		GLUniformBuffer animation("Animation", 1, 100 * sizeof(Matrix4f));
		animation.bind(reinterpret_cast<GLProgram*>(program.get()));
		Matrix4f bones[100];
		animation.write(bones, 100 * sizeof(Matrix4f));
		*/

		// 根据模型大小设置相机移动速度
		auto speed = 0.f;
		speed += model.aabb.max.x - model.aabb.min.x;
		speed += model.aabb.max.y - model.aabb.min.y;
		speed += model.aabb.max.z - model.aabb.min.z;
		speed /= 3;
		controller.setSpeed(speed * 0.5);

		bool running    = true;
		window->onClose = [&] { running = false; };
		window->onKey   = [&](int action, Key key) {
            if(!action)
                return;

            switch(key)
            {
            case Key::Escape:
                running = false;
                break;

            case Key::F11:
                window->setFullscreen(!window->isFullscreen());
                break;

            case Key::P:
                window->setCursorLock(false);
                break;
            }
		};
		window->onScroll = [&](Vector2d offset) {
			static float fov = degrees(camera.getVFOV());
			if(1.f <= fov && fov <= 60.f)
				fov -= (float)offset.y * 3 * (fov / 60);
			fov = std::clamp(fov, 1.f, 60.f);
			camera.setPerspective(radians(fov), camera.getAspectRatio(), camera.getNear(), camera.getFar());
		};
		window->onResize = [&](Vector2i size) {
			camera.setPerspective(camera.getVFOV(), (float)size.x / size.y, camera.getNear(), camera.getFar());
		};
		window->setVisible(true);

		window->setCursorLock(true);
		window->setRawMouseMotion(true);

		ui::Window ATT("ATT"); // 摄像机姿态信息
		ui::Label  position;   // 坐标
		ui::Label  angles;     // 姿态角角度
		ATT.add(position);
		ATT.add(angles);

		ui::NodeEditor editor;

		Clock clock;
		while(running)
		{
			const auto dt = (float)clock.getSeconds();
			clock.restart();
#if 0
			static float elapse = 0;
			elapse += dt;
			while(elapse >= 0.002f)
			{
				controller.update(0.002f);
				elapse -= 0.002f;
			}
#else
			controller.update(dt);
#endif

			UI::beginFrame();

			editor.update();

			const auto& pos = camera.getPosition();
			position.setText(fmt::format("X    : {: .2f}\n"
			                             "Y    : {: .2f}\n"
			                             "Z    : {: .2f}\n",
			                             pos.x, pos.y, pos.z));
			const auto& dir = camera.getRotation();
			angles.setText(fmt::format("Roll : {: .1f}\n"
			                           "Pitch: {: .1f}\n"
			                           "Yaw  : {: .1f}\n",
			                           dir.z, dir.x, dir.y));
			ATT.update();

			matrices->getBuffer().map();
			matrices->getBuffer().write(camera.getView().data(), sizeof(Matrix4f));
			matrices->getBuffer().write(camera.getProjection().data(), sizeof(Matrix4f), sizeof(Matrix4f));
			matrices->getBuffer().write(Matrix4f().data(), sizeof(Matrix4f), 2 * sizeof(Matrix4f));
			matrices->getBuffer().unmap();

			cmdBuffer->begin();
			{
				cmdBuffer->setViewport({window->getSize()});
				cmdBuffer->setClearColor({0, 0, 0, 0});
				cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

				cmdBuffer->setPipeline(pipeline);
				for(const auto& mesh : model.meshs)
				{
					const auto vb = mesh.vertexBuffer;
					const auto ib = mesh.indexBuffer;

					cmdBuffer->setVertexBuffer(vb);
					cmdBuffer->setIndexBuffer(ib);
					cmdBuffer->drawIndexed(ib->getCount());
				}
			}
			cmdBuffer->end();
			cmdQueue->submit(cmdBuffer);

			UI::endFrame();

			window->update();
		}

		return 0;
	}
};

int main(int argc, char* argv[])
{
	Viewer instance;
	return instance.run(argc, argv);
}

/*
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
*/
