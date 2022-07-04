// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

#include <thread>

#if TARGET_OS == OS_WIN
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#elif TARGET_OS == OS_LINUX
#include <stdio.h>
#endif

#define FMT_HEADER_ONLY
#include <fmt/core.h>

std::filesystem::path OpenFile()
{
#if TARGET_OS == OS_WIN
	char buffer[MAX_PATH] = {};
	OPENFILENAMEA ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFilter = "Model files\0*.gltf;*.glb;*.obj;*.fbx\0";
	ofn.lpstrFile = buffer;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a Model file to load";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if(GetOpenFileNameA(&ofn))
	{
		return buffer;
	}
#elif TARGET_OS == OS_LINUX
	char buffer[1024];
	FILE* file = popen("zenity --title=\"Select a glTF file to load\" --file-filter=\"glTF files | *.gltf *.glb\" --file-selection", "r");
	if(file)
	{
		std::string filename;
		while(fgets(buffer, sizeof(buffer), file))
		{
			filename += buffer;
		};
		filename.erase(std::remove(filename.begin(), filename.end(), '\n'), filename.end());
		return filename;
	}
#else
	return "";
#endif
}

void PrintMonitorInfo();
void PrintRendererInfo();
void PrintModelInfo(const Model&);

int main(int argc, char* argv[])
{
	try
	{
		std::filesystem::current_path(std::filesystem::path(argv[0]).parent_path());

		Renderer::setAPI(Renderer::API::OpenGL);
		Window::init();

		{
			Window window("Test", Monitor::getPrimary()->getSize() / 2);
			Renderer::init(window);
			UI::init(window);
			Input::setWindow(window);

			{
				PrintMonitorInfo();
				PrintRendererInfo();

				auto program = Program::create("Shaders/mesh");

				PipelineLayout layout = {
					{"albedo",    0, PipelineLayout::Type::Sampler, PipelineLayout::StageFlags::Fragment},
					{"roughness", 1, PipelineLayout::Type::Sampler, PipelineLayout::StageFlags::Fragment},
					{"ao",        2, PipelineLayout::Type::Sampler, PipelineLayout::StageFlags::Fragment},
					{"emissive",  3, PipelineLayout::Type::Sampler, PipelineLayout::StageFlags::Fragment},
					{"normal",    4, PipelineLayout::Type::Sampler, PipelineLayout::StageFlags::Fragment}
				};
				Pipeline::Descriptor desc;
				desc.layout = layout;
				desc.program = program;
				auto pipeline = Pipeline::create(desc);

				auto cmdQueue = CommandQueue::create();
				auto cmdBuffer = CommandBuffer::create();

				Camera camera(Camera::Type::Perspective);
				camera.setPerspective(radians(45.f), (float)window.getSize().x / window.getSize().y, 0.1f, 5000.f);

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

				std::filesystem::path path;
				if(argc > 1)
					path = argv[1];
				else
				{
					// path = "../../../../../../Model/bee.glb";
					// path = "../../../../../../Model/sponza/sponza.obj";
					path = "../../../../../../Model/sponza/sponza.glb";
					// path = "../../../../../../Model/m4a1/m4a1.gltf";
					// path = "../../../../../../Model/pistol/kimber_desert_warrior/scene.gltf";
					// path = "../../../../../../Model/Bistro/BistroExterior.glb";
					// path = "../../../../../../Model/SpeedTree/White Oak/HighPoly/White_Oak.fbx";
					// path = "../../../../../../Model/San_Miguel/san-miguel-low-poly.obj";
					// path = "../../../../../../Model/San_Miguel/SanMiguel-low.glb";
				}

				Model model;
				model.load(path, Model::ProcessFlags::Fast, [](float progress) {
					printf("Meshes loading: %.1f%%  \r", progress * 100);
				});
				PrintModelInfo(model);

				// 根据模型大小设置相机移动速度
				auto speed = 0.f;
				speed += model.getAABB().max.x - model.getAABB().min.x;
				speed += model.getAABB().max.y - model.getAABB().min.y;
				speed += model.getAABB().max.z - model.getAABB().min.z;
				speed /= 3;
				controller.setSpeed(speed * 0.5);

				bool running = true;
				window.onClose = [&]{ running = false; };
				window.onKey = [&](int action, Key key)
				{
					if(!action)
						return;

					switch(key)
					{
					case Key::Escape:
						running = false;
						break;

					case Key::F11:
						window.setFullscreen(!window.isFullscreen());
						break;

					case Key::O:
						model.compress();
						break;

					case Key::I:
						model.decompress();
						break;

					case Key::LeftAlt:
						window.setCursorLock(!window.isCursorLock());
						break;
					}
				};
				window.onScroll = [&](Vector2d offset)
				{
					static float fov = degrees(camera.getVFOV());
					if(1.f <= fov && fov <= 60.f)
						fov -= (float)offset.y * 3 * (fov / 60);
					fov = std::clamp(fov, 1.f, 60.f);
					camera.setPerspective(radians(fov), camera.getAspectRatio(), camera.getNear(), camera.getFar());
				};
				window.onResize = [&](Vector2i size)
				{
					camera.setPerspective(camera.getVFOV(), (float)size.x / size.y, camera.getNear(), camera.getFar());
				};
				window.setVisible(true);

				window.setCursorLock(true);
				window.setRawMouseMotion(true);

				ui::Window ATT("ATT"); // 摄像机姿态信息
				ui::Label position;    // 坐标
				ui::Label angles;      // 姿态角角度
				ui::Button open("Open File");
				ATT.add(position);
				ATT.add(angles);
				ATT.add(open);

				open.click = [&model](auto btn) {
					const auto path = OpenFile();
					model.load(path, Model::ProcessFlags::Fast, [](float progress) {
						printf("Meshes loading: %.1f%%  \r", progress * 100);
					});
					PrintModelInfo(model);
				};

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

					const auto& pos = camera.getPosition();
					position.setText(fmt::format(
						"X    : {: .2f}\n"
						"Y    : {: .2f}\n"
						"Z    : {: .2f}\n",
						pos.x, pos.y, pos.z));
					const auto& dir = camera.getRotation();
					angles.setText(fmt::format(
						"Roll : {: .1f}\n"
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
						cmdBuffer->setViewport({window.getSize()});
						cmdBuffer->setClearColor({0, 0, 0, 0});
						cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

						cmdBuffer->setPipeline(pipeline);
						for(const auto& mesh : model.getMeshes())
						{
							if(mesh.isCompressed())
								continue;

							const auto vb = mesh.getVertexBuffer();
							const auto ib = mesh.getIndexBuffer();

							cmdBuffer->setVertexBuffer(vb);
							cmdBuffer->setIndexBuffer(ib);
							cmdBuffer->drawIndexed(ib->getCount());
						}
					}
					cmdBuffer->end();
					cmdQueue->submit(cmdBuffer);

					UI::endFrame();

					window.update();
				}
			}

            UI::deinit();
			Renderer::deinit();
		}

		Window::deinit();
	}
	catch(const std::runtime_error& e)
	{
		puts(e.what());
		exit(1);
	}

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

void PrintModelInfo(const Model& model)
{
	const auto& info = model.getMeshInfo();
	printf("%s", fmt::format(
		"Mesh\n"
		"|-Name     : {}\n"
		"|-Triangles: {}\n"
		"|-Vertices : {}\n"
		"`-Indices  : {}\n",
		model.getName(), info.triangles, info.vertices, info.indices).c_str());
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
