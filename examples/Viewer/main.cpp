// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "../Base/Base.hpp"

#include <glad/glad.h>

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
			path = "../../../../../../Model/sponza/sponza.obj";
			// path = "../../../../../../Model/san_miguel/san-miguel-low-poly.obj";

			// PBR
			path = "../../../../../../Model/pbr/sponza/sponza.glb";
			// path = "../../../../../../Model/m4a1/m4a1.gltf";
			// path = "../../../../../../Model/pistol/kimber_desert_warrior/scene.gltf";
			path = "../../../../../../Model/pbr/MetalRoughSpheres/MetalRoughSpheres.gltf";
			// path = "../../../../../../Model/ORCA/bistro/BistroExterior.glb";
			// path = "../../../../../../Model/ORCA/bistro/BistroExterior.fbx";
			// path = "../../../../../../Model/ORCA/SunTemple/SunTemple.fbx";
		}

		ModelImporter importer;
		auto          model = importer.load(path);
		printModelInfo(model);

		// model.meshes.clear();
		// model.meshes.push_back(Primitive::makeSphere(64, 32).value());
		// model.meshs.push_back(Primitive::makeIcoSphere(7));
		// model.meshs.push_back(Primitive::makeCapsule(15, 2, 0.5).value());
		// model.meshs.push_back(Primitive::makePlane(10, 10).value());

		auto program = Program::create("Shaders/mesh");
		// auto program = Program::create("Shaders/pbr");

		PipelineLayout layout = {{"albedo", 0, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		                         {"roughness", 1, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		                         {"ao", 2, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		                         {"emissive", 3, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		                         {"normal", 4, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment}};
		Pipeline::Descriptor desc;
		desc.layout   = layout;
		desc.program  = program;
		auto pipeline = Pipeline::create(desc);

		auto cmdQueue  = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.setPerspective(radians(45.f), (float)window->getSize().x / window->getSize().y, 0.1f, 2000.f);

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
		controller.setSpeed(speed * 0.4);

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

            case Key::LeftAlt:
                window->setCursorLock(!window->isCursorLock());
                break;

            case Key::F11:
                window->setFullscreen(!window->isFullscreen());
                break;

            case Key::F12:
                std::vector<char> buf(3 * window->getSize().x * window->getSize().y);
                glReadPixels(0, 0, window->getSize().x, window->getSize().y, GL_RGB, GL_UNSIGNED_BYTE, buf.data());
                auto img = Image(buf.data(), buf.size(), window->getSize(), 3);
                img.flipVertically();
                img.save("screenshot.png");
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

		ui::Window attInfo("ATT"); // 摄像机姿态信息
		ui::Label  position;       // 坐标
		ui::Label  angles;         // 姿态角角度
		attInfo.add(position);
		attInfo.add(angles);

		Timer timer;
		while(running)
		{
			const auto dt = (float)timer.getSeconds();
			timer.restart();
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
			attInfo.update();

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
				for(const auto& mesh : model.meshes)
				{
					const auto vb = mesh.vertexBuffer;
					const auto ib = mesh.indexBuffer;

					cmdBuffer->setVertexBuffer(vb);
					cmdBuffer->setIndexBuffer(ib);

					if(mesh.material)
					{
						if(mesh.material->pbr.albedo)
						{
							cmdBuffer->setTexture(mesh.material->pbr.albedo, 0);
							cmdBuffer->setTexture(mesh.material->pbr.normals, 1);
							cmdBuffer->setTexture(mesh.material->pbr.metallic, 2);
							cmdBuffer->setTexture(mesh.material->pbr.roughness, 3);
							// cmdBuffer->setTexture(mesh.material->pbr.emissive, 4);
							// cmdBuffer->setTexture(mesh.material->pbr.occlusion, 5);
						}
						else
						{
							cmdBuffer->setTexture(mesh.material->diffuse, 0);
							cmdBuffer->setTexture(mesh.material->specular, 1);
							cmdBuffer->setTexture(mesh.material->ambient, 2);
							cmdBuffer->setTexture(mesh.material->emissive, 3);
							cmdBuffer->setTexture(mesh.material->height, 4);
							cmdBuffer->setTexture(mesh.material->normals, 5);
							cmdBuffer->setTexture(mesh.material->shininess, 6);
							cmdBuffer->setTexture(mesh.material->opacity, 7);
						}
					}

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
