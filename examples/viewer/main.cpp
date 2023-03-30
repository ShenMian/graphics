// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "../base/base.hpp"

#include <array>

#include <imgui.h>

#include <ImGuizmo.h>

#include <glad/glad.h>

struct MatricesBuffer
{
	Matrix4 view;
	Matrix4 proj;
	Matrix4 model;
};

struct MaterialBuffer
{
	Vector4 base_color_factor;
	Vector4 emissive_factor;
	float   metallicFactor;
	float   roughnessFactor;

	int albedo_map_set;
	int normal_map_set;
	int metallic_map_set;
	int roughness_map_set;
	int emissive_map_set;
	int occlusion_map_set;

	int workflow;
};

struct alignas(16) DirectionalLight
{
	Vector3 color;
	float   intensity;
	Vector3 direction;
};

void DrawChar(Font& font, unsigned long code, const Vector2f& pos, std::shared_ptr<CommandBuffer> cmdBuf)
{
	auto getTexture = [&](unsigned long code) {
		static std::unordered_map<unsigned long, std::shared_ptr<Texture>> map;

		auto it = map.find(code);
		if(it != map.end())
			return it->second;
		std::shared_ptr<Texture> texture;
		Image                    flippedImage = font.get_glyph(code, 32).image;
		flippedImage.flip_vertically();
		texture = Texture::create(flippedImage);
		map.insert({code, texture});
		return texture;
	};

	auto       texture = getTexture(code);
	const auto size    = font.get_glyph(code, 32).image.size();
	// clang-format off
	float vertices[6][4] = {
		{pos.x(),            pos.y() + size.y(), 0.0, 0.0},
		{pos.x(),            pos.y(),            0.0, 1.0},
		{pos.x() + size.x(), pos.y(),            1.0, 1.0},

		{pos.x(),            pos.y() + size.y(), 0.0, 0.0},
		{pos.x() + size.x(), pos.y(),            1.0, 1.0},
		{pos.x() + size.x(), pos.y() + size.y(), 1.0, 0.0}};
	// clang-format on
	VertexFormat fmt          = {{{"position", Format::RG32F}, {"tex_coord", Format::RG32F}}};
	auto         vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices), fmt, Buffer::Usage::Dynamic);

	auto           program = Program::create("shaders/text2d");
	PipelineLayout layout  = {{"albedo_map", 0, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment}};
	Pipeline::Descriptor desc;
	// desc.rasterizer.polygonMode = PolygonMode::Wireframe;
	desc.layout   = layout;
	desc.program  = program;
	auto pipeline = Pipeline::create(desc);

	cmdBuf->set_pipeline(pipeline);
	cmdBuf->set_texture(texture, 0);
	cmdBuf->set_vertex_buffer(vertexBuffer);
	cmdBuf->draw(6);
}

void DrawString(std::string_view str)
{
}

void GLScreenshot(const Vector2i& size)
{
	std::vector<GLubyte> buf(3 * size.x() * size.y());
	glReadPixels(0, 0, size.x(), size.y(), GL_RGB, GL_UNSIGNED_BYTE, buf.data());
	auto img = Image(buf.data(), buf.size(), size, 3);
	img.flip_vertically();
	img.save("screenshot.png");
}

#define PBR 1

class Viewer final : public Base
{
public:
	Viewer() : Base("Viewer"){};

	int main(int argc, char* argv[])
	{
		// Font font("C:/Users/sms/AppData/Local/Microsoft/Windows/Fonts/CascadiaCode.ttf");

		fs::path path = "../../../../../../Model/";
		if(argc > 1)
			path = argv[1];
		else
		{
#if PBR
			// Scene
			path += "glTF-Sample-Models/Sponza/glTF/Sponza.gltf";
			// path += "ORCA/Bistro/BistroExterior.glb";
			// path += "ORCA/Bistro/BistroExterior.fbx";
			// path += "ORCA/SunTemple/SunTemple.fbx";

			// Model
			// path += "glTF-Sample-Models/FlightHelmet/glTF/FlightHelmet.gltf";
			// path += "m4a1/m4a1.gltf";
			// path += "pistol/kimber_desert_warrior/scene.gltf";

			// Basic test
			// path += "glTF-Sample-Models/MetalRoughSpheres/glTF/MetalRoughSpheres.gltf";
			// path += "glTF-Sample-Models/NormalTangentTest/glTF/NormalTangentTest.gltf";
			// path += "glTF-Sample-Models/NormalTangentMirrorTest/glTF/NormalTangentMirrorTest.gltf";
			// path += "glTF-Sample-Models/TextureLinearInterpolationTest/glTF/TextureLinearInterpolationTest.gltf";
			// path += "glTF-Sample-Models/TextureCoordinateTest/glTF/TextureCoordinateTest.gltf";
			// path += "glTF-Sample-Models/TextureSettingsTest/glTF/TextureSettingsTest.gltf";
			// path += "glTF-Sample-Models/VertexColorTest/glTF/VertexColorTest.gltf";
			// path += "glTF-Sample-Models/AlphaBlendModeTest/glTF/AlphaBlendModeTest.gltf";

			// Feature test
			// path += "glTF-Sample-Models/AttenuationTest/glTF/AttenuationTest.gltf";
			// path += "glTF-Sample-Models/ClearcoatTest/glTF/ClearcoatTest.gltf";
			// path += "glTF-Sample-Models/EmissiveStrengthTest/glTF/EmissiveStrengthTest.gltf";
			// path += "glTF-Sample-Models/SpecGlossVsMetalRough/glTF/SpecGlossVsMetalRough.gltf";
			// path += "glTF-Sample-Models/TextureTransformTest/glTF/TextureTransformTest.gltf";
			// path += "glTF-Sample-Models/TextureTransformMultiTest/glTF/TextureTransformMultiTest.gltf";
#else
			path = "sponza/sponza.obj";
			// path = "dishonored_2/scene.glb";
			// path = "san_miguel/san-miguel-low-poly.obj";
#endif
		}

#if PBR
		auto program = Program::create("shaders/pbr");
		// clang-format off
		PipelineLayout layout  = {
		     {"albedo_map", 1, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"normal_map", 2, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"metallic_map", 3, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"roughness_map", 4, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"emissive_map", 5, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		     {"occlusion_map", 6, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment}};
		// clang-format on
#else
		auto program = Program::create("shaders/phong");
		PipelineLayout layout = {
		    {"diffuse_map", 2, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"specular_map", 3, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"ambient_map", 4, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"emissive_map", 5, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"height_map", 6, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"normal_map", 7, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"shininess_map", 8, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment},
		    {"opacity_map", 9, PipelineLayout::Type::Texture, PipelineLayout::StageFlags::Fragment}};
#endif
		Pipeline::Descriptor desc;
		desc.layout   = layout;
		desc.program  = program;
		auto pipeline = Pipeline::create(desc);

		auto cmdQueue  = CommandQueue::create();
		auto cmdBuffer = CommandBuffer::create();

		Camera camera(Camera::Type::Perspective);
		camera.set_perspective(radians(60.f), static_cast<float>(window->get_size().x()) / window->get_size().y(), 0.1f, 2000.f);
		const auto fov = degrees(camera.get_hfov());

		Gamepad gamepad(0);

		Controller controller;
		controller.set_camera(camera);
		controller.set_gamepad(gamepad);

		auto matrices = UniformBuffer::create(0, sizeof(MatricesBuffer));
#if PBR
		// auto material = UniformBuffer::create(7, sizeof(MaterialBuffer));
#else
		auto lights = UniformBuffer::create(1, sizeof(DirectionalLight) + sizeof(uint32_t));
#endif

		/*
		GLUniformBuffer animation("Animation", 1, 100 * sizeof(Matrix4f));
		animation.bind(static_cast<GLProgram*>(program.get()));
		Matrix4f bones[100];
		animation.write(bones, 100 * sizeof(Matrix4f));
		*/

		Model model;
		auto  loadModel = [&](const fs::path& path) {
            ModelImporter importer;
            model = importer.load(path, [](float progress) {
                fmt::print(":: Model loading: {:.1f}%  \r", progress * 100);
                if(progress == 1.f)
                    printf("\n");
            });
            print_model_info(model);

            // 根据模型大小设置相机移动速度
            auto speed = 0.f;
            speed += model.aabb.max().x() - model.aabb.min().x();
            speed += model.aabb.max().y() - model.aabb.min().y();
            speed += model.aabb.max().z() - model.aabb.min().z();
            speed /= 3;
            controller.set_speed(speed * 0.4);
		};

		loadModel(path);

		// model.save("D:/Users/sms/Desktop/test.model");
		// model.meshes.clear();
		// model.load("D:/Users/sms/Desktop/test.model");

		// model.meshes.clear();
		// model.meshes.push_back(Primitive::makeUVSphere(64, 32).value());
		// model.meshes.push_back(Primitive::makeIcoSphere(7));
		// model.meshes.push_back(Primitive::makeCapsule(15, 2, 0.5).value());
		// model.meshes.push_back(Primitive::makePlane(10, 10).value());

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
                window->set_cursor_lock(!window->is_cursor_lock());
                break;

            case Key::F11:
                window->set_fullscreen(!window->is_fullscreen());
                break;

            case Key::PrintScreen:
                GLScreenshot(window->get_size());
                break;

            default:
                break;
            }
		};
		window->onScroll = [&](Vector2d offset) {
			static float fov = degrees(camera.get_vfov());
			if(1.f <= fov && fov <= 60.f)
				fov -= offset.y() * 3.f * (fov / 60);
			fov = std::clamp(fov, 1.f, 60.f);
			camera.set_perspective(radians(fov), camera.get_aspect_ratio(), camera.get_near(), camera.get_far());
		};
		window->onResize = [&](Vector2i size) {
			camera.set_perspective(camera.get_vfov(), static_cast<float>(size.x()) / size.y(), camera.get_near(),
			                      camera.get_far());
		};
		window->onDrop = [&](int, const char* paths[]) { loadModel(paths[0]); };
		window->set_visible(true);

		window->set_cursor_lock(true);
		window->set_raw_mouse_motion(true);

		ui::Window performance("Performance");
		ui::Label  fps;
		ui::Label  gpu;
		performance.add(fps);
		performance.add(gpu);

		ui::Window attInfo("ATT"); // 摄像机姿态信息
		ui::Label  position;       // 坐标
		ui::Label  angles;         // 姿态角角度
		attInfo.add(position);
		attInfo.add(angles);

		Matrix4f transform; // TODO

		Timer timer;
		while(running)
		{
			const float dt = timer.get_seconds();
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

			beginFrame();

			{
				static size_t                 fpsRecordsCount   = 0;
				static std::array<float, 100> dtRecords         = {0};
				dtRecords[fpsRecordsCount++ % dtRecords.size()] = dt;
				const auto avgDt  = std::accumulate(dtRecords.begin(), dtRecords.end(), 0.f) / dtRecords.size();
				const auto avgFps = 1.f / avgDt;

				static size_t                 gpuRecordsCount     = 0;
				static std::array<float, 100> gpuRecords          = {0};
				gpuRecords[gpuRecordsCount++ % gpuRecords.size()] = dt * 1000.f;
				const auto avgGPU = std::accumulate(gpuRecords.begin(), gpuRecords.end(), 0.f) / gpuRecords.size();

				fps.set_text(fmt::format("FPS: {}\n", static_cast<int>(avgFps)));
				gpu.set_text(fmt::format("GPU: {:.2f} ms\n", avgGPU));
				performance.update();
			}

			{
				const auto& pos = camera.get_position();
				position.set_text(fmt::format("X    : {: .2f}\n"
				                             "Y    : {: .2f}\n"
				                             "Z    : {: .2f}\n",
				                             pos.x(), pos.y(), pos.z()));
				const auto& dir = camera.get_rotation();
				angles.set_text(fmt::format("Roll : {: .1f}\n"
				                           "Pitch: {: .1f}\n"
				                           "Yaw  : {: .1f}\n",
				                           dir.z(), dir.x(), dir.y()));
				attInfo.update();
			}

			matrices->get_buffer().map();
			{
				auto ptr   = static_cast<MatricesBuffer*>(matrices->get_buffer().get_data());
				ptr->view  = camera.get_view();
				ptr->proj  = camera.get_projection();
				ptr->model = transform;
			}
			matrices->get_buffer().unmap();

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
				cmdBuffer->set_viewport({window->get_size()});
				cmdBuffer->set_clear_color({0, 0, 0, 0});
				cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

				cmdBuffer->set_pipeline(pipeline);
				for(const auto& mesh : model.meshes)
				{
					const auto vb = mesh.vertexBuffer;
					const auto ib = mesh.indexBuffer;

					cmdBuffer->set_vertex_buffer(vb);
					cmdBuffer->set_index_buffer(ib);

					if(mesh.material)
					{
#if PBR
						cmdBuffer->set_texture(mesh.material->pbr.albedo, 1);
						cmdBuffer->set_texture(mesh.material->pbr.normals, 2);
						cmdBuffer->set_texture(mesh.material->pbr.metallic, 3);
						cmdBuffer->set_texture(mesh.material->pbr.roughness, 4);
						cmdBuffer->set_texture(mesh.material->pbr.emissive, 5);
						cmdBuffer->set_texture(mesh.material->pbr.occlusion, 6);
#else
						cmdBuffer->setTexture(mesh.material->diffuse, 2);
						cmdBuffer->setTexture(mesh.material->specular, 3);
						cmdBuffer->setTexture(mesh.material->ambient, 4);
						cmdBuffer->setTexture(mesh.material->emissive, 5);
						cmdBuffer->setTexture(mesh.material->height, 6);
						cmdBuffer->setTexture(mesh.material->normals, 7);
						cmdBuffer->setTexture(mesh.material->shininess, 8);
						cmdBuffer->setTexture(mesh.material->opacity, 9);
#endif
					}

					cmdBuffer->draw_indexed(ib->get_count());
				}

				// DrawChar(font, 'A', {0, 0}, cmdBuffer);
			}
			cmdBuffer->end();
			cmdQueue->submit(cmdBuffer);

			endFrame();
		}

		return 0;
	}

	void beginFrame()
	{
		UI::begin_frame();
		ImGuizmo::BeginFrame();
	}

	void endFrame()
	{
		UI::end_frame();
		window->update();
	}
};

int main(int argc, char* argv[])
{
	Viewer instance;
	return instance.run(argc, argv);
}
