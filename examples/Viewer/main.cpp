// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "../Base/Base.hpp"

#include <imgui.h>

#include <ImGuizmo.h>

#include <glad/glad.h>

struct alignas(16) DirectionalLight
{
	Vector3 color;
	float   intensity;
	Vector3 direction;
};

#define PBR 1

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
#if PBR
			path = "../../../../../../Model/pbr/sponza/sponza.glb";
			// path = "../../../../../../Model/m4a1/m4a1.gltf";
			// path = "../../../../../../Model/pistol/kimber_desert_warrior/scene.gltf";
			// path = "../../../../../../Model/pbr/MetalRoughSpheres/MetalRoughSpheres.gltf";
			// path = "../../../../../../Model/pbr/FlightHelmet/FlightHelmet.gltf";
			// path = "../../../../../../Model/ORCA/Bistro/BistroExterior.glb";
			// path = "../../../../../../Model/ORCA/Bistro/BistroExterior.fbx";
			// path = "../../../../../../Model/ORCA/SunTemple/SunTemple.fbx";
#else
			path = "../../../../../../Model/sponza/sponza.obj";
			// path = "../../../../../../Model/dishonored_2/scene.glb";
			// path = "../../../../../../Model/san_miguel/san-miguel-low-poly.obj";
#endif
		}

		ModelImporter importer;
		auto          model = importer.load(path);
		printModelInfo(model);

		// model.meshes.clear();
		// model.meshes.push_back(Primitive::makeUVSphere(64, 32).value());
		// model.meshs.push_back(Primitive::makeIcoSphere(7));
		// model.meshs.push_back(Primitive::makeCapsule(15, 2, 0.5).value());
		// model.meshs.push_back(Primitive::makePlane(10, 10).value());

#if PBR
		auto           program = Program::create("Shaders/pbr");
		PipelineLayout layout  = {
		     {"albedo_map", 0, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"normal_map", 1, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"metallic_map", 2, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"roughness_map", 3, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"emissive_map", 4, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     { "occlusion_map",
			   5,
			   PipelineLayout::Type::Texture,
			   PipelineLayout::StageFlags::Fragment }};
#else
		// clang-format off
		auto           program = Program::create("Shaders/phong");
		PipelineLayout layout  = {
		     {"diffuse_map", 0, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"specular_map", 1, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"ambient_map", 2, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"emissive_map", 3, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"height_map", 4, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"normal_map", 5, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"shininess_map", 6, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"opacity_map", 7, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment}};
		// clang-format on
#endif
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
		auto lights   = UniformBuffer::create(1, sizeof(DirectionalLight) + sizeof(uint32_t));

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

		Matrix4f transform;

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
			ImGuizmo::BeginFrame();

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
			matrices->getBuffer().write(transform.data(), sizeof(Matrix4f), 2 * sizeof(Matrix4f));
			matrices->getBuffer().unmap();

			// ImGuizmo::DrawGrid(camera.getView().data(), camera.getProjection().data(), Matrix4().data(), 10);

#if 0
			float translation[3], rotation[3], scale[3];
			ImGuizmo::DecomposeMatrixToComponents(transform.data(), translation, rotation, scale);
			ImGui::InputFloat3("Tr", translation);
			ImGui::InputFloat3("Rt", rotation);
			ImGui::InputFloat3("Sc", scale);
			ImGuizmo::RecomposeMatrixFromComponents(translation, rotation, scale, transform.data());

			// TODO: DEBUG
			// ImGuizmo::DrawCubes(camera.getView().data(), camera.getProjection().data(), transform.data(), 1);

			Matrix4 delta; // 逆变换, 用于撤回操作

			ImGuiIO& io = ImGui::GetIO();
			ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
			ImGuizmo::Manipulate(camera.getView().data(), camera.getProjection().data(), ImGuizmo::OPERATION::TRANSLATE,
			                     ImGuizmo::WORLD, transform.data(), delta.data());
#endif

			/*
			std::vector<DirectionalLight> dirLights;
			dirLights.push_back(DirectionalLight{
			    .color     = Vector3(0.0),
			    .intensity = 1.f,
			    .direction = Vector3(-1.0, 0.0, -0.5),
			});
			size_t dirLightsSize = dirLights.size();
			lights->getBuffer().map();
			lights->getBuffer().write(dirLights.data(), dirLightsSize * sizeof(DirectionalLight));
			lights->getBuffer().write(&dirLightsSize, sizeof(uint32_t), dirLightsSize * sizeof(DirectionalLight));
			lights->getBuffer().unmap();
			*/

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
#if PBR
						cmdBuffer->setTexture(mesh.material->pbr.albedo, 0);
						cmdBuffer->setTexture(mesh.material->pbr.normals, 1);
						cmdBuffer->setTexture(mesh.material->pbr.metallic, 2);
						cmdBuffer->setTexture(mesh.material->pbr.roughness, 3);
						cmdBuffer->setTexture(mesh.material->pbr.emissive, 4);
						cmdBuffer->setTexture(mesh.material->pbr.occlusion, 5);
#else
						cmdBuffer->setTexture(mesh.material->diffuse, 0);
						cmdBuffer->setTexture(mesh.material->specular, 1);
						cmdBuffer->setTexture(mesh.material->ambient, 2);
						cmdBuffer->setTexture(mesh.material->emissive, 3);
						cmdBuffer->setTexture(mesh.material->height, 4);
						cmdBuffer->setTexture(mesh.material->normals, 5);
						cmdBuffer->setTexture(mesh.material->shininess, 6);
						cmdBuffer->setTexture(mesh.material->opacity, 7);
#endif
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
