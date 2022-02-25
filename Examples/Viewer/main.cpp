// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Graphics.h"

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "OpenGL/GLUniformBuffer.h"

using namespace std::literals::string_literals;

void PrintInfo();

int main()
{
	try
	{
		Renderer::setAPI(Renderer::API::OpenGL);
		Window::init();

		{
			Window window("Test", Monitor::getPrimary().getSize() / 2);
			Renderer::init(window);
			UI::init(window);
			Input::setWindow(window);

			{
				PrintInfo();

				const std::filesystem::path path = "../../../model/low_poly/akm/scene.gltf";

				Model model;
				model.load(path, Model::ProcessFlags::Fast);

				Program::Descriptor programDesc;
				{
					Shader::Descriptor vertShaderDesc;
					vertShaderDesc.stage = Shader::Stage::Vertex;
					vertShaderDesc.path = "Shaders/pbr.vert.glsl";
					programDesc.vertex = Shader::create(vertShaderDesc);

					Shader::Descriptor fragShaderDesc;
					fragShaderDesc.stage = Shader::Stage::Fragment;
					fragShaderDesc.path = "Shaders/pbr.frag.glsl";
					programDesc.fragment = Shader::create(fragShaderDesc);
				}
				auto program = Program::create(programDesc);

				PipelineLayout layout = {
					{"albedo",    PipelineLayout::Type::Texture, 0, PipelineLayout::StageFlags::Fragment},
					{"roughness", PipelineLayout::Type::Texture, 1, PipelineLayout::StageFlags::Fragment},
					{"ao",        PipelineLayout::Type::Texture, 2, PipelineLayout::StageFlags::Fragment},
					{"emissive",  PipelineLayout::Type::Texture, 3, PipelineLayout::StageFlags::Fragment},
					{"normal",    PipelineLayout::Type::Texture, 4, PipelineLayout::StageFlags::Fragment}
				};
				Pipeline::Descriptor desc;
				desc.layout = layout;
				desc.program = program;
				auto pipeline = Pipeline::create(desc);

				auto cmdQueue = CommandQueue::create();
				auto cmdBuffer = CommandBuffer::create();

				Camera camera(Camera::Type::Perspective);
				camera.setPerspective(radians(45.f), (float)window.getSize().x / window.getSize().y, 0.1f, 5000.f);

				Controller controller;
				controller.setCamera(camera);
				Gamepad gamepad(0);
				controller.setGamepad(gamepad);

				bool running = true;
				window.onClose = [&]() { running = false; };
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

					case Key::P:
						window.setCursorLock(false);
						break;
					}
				};
				window.onScroll = [&](Vector2d offset)
				{
					static float fov = degrees(camera.getVFOV());
					if(1.f <= fov && fov <= 60.f)
						fov -= (float)offset.y * 3 * (fov / 60);
					fov = std::clamp(fov, 1.f, 60.f);
					camera.setPerspective(radians(fov), (float)window.getSize().x / window.getSize().y, 0.03f, 10000.f);
				};
				window.onResize = [&](Vector2i size)
				{
					camera.setPerspective(radians(camera.getVFOV()), (float)size.x / size.y, 0.1f, 5000.0f);
				};
				window.setVisible(true);

				window.setCursorLock(true);
				window.setRawMouseMotion(true);

				ui::Window ATT("ATT"); // 摄像机姿态信息
				ui::Label position;    // 坐标
				ui::Label angles;      // 姿态角角度
				ATT.add(position);
				ATT.add(angles);

				GLUniformBuffer uniformBuffer("Camera", 0, 2 * sizeof(Matrix4f));
				uniformBuffer.bind(reinterpret_cast<GLProgram*>(program.get()));

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

					uniformBuffer.write(camera.getView().data(), sizeof(Matrix4f));
					uniformBuffer.write(camera.getProjection().data(), sizeof(Matrix4f), sizeof(Matrix4f));

					program->setUniform("model", Matrix4f());

					cmdBuffer->begin();
					{
						cmdBuffer->setViewport({window.getSize()});
						cmdBuffer->setClearColor({0, 0, 0, 0});
						cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);

						cmdBuffer->setPipeline(pipeline);
						for(const auto& mesh : model.getMeshes())
						{
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
	catch(std::runtime_error& e)
	{
		puts(e.what());
		exit(1);
	}

	return 0;
}

void PrintInfo()
{
    // 打印显示器信息
    for(const auto& mon : Monitor::getMonitors())
    {
        printf("%s", fmt::format(
                "Monitor\n"
                "|-Name        : {}\n"
                "|-Size        : {}x{}\n"
                "`-Refresh rate: {} Hz\n",
                mon.getName(), mon.getSize().x, mon.getSize().y, mon.getRefreshRate()).c_str());
    }

    // 打印基本信息
    const auto renderer = Renderer::get();
    printf("%s", fmt::format(
            "Basic\n"
            "|-Device  : {}\n"
            "|-Renderer: {}\n"
            "`-Vendor  : {}\n",
            renderer->getDeviceName(), renderer->getRendererName(), renderer->getVendorName()).c_str());
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
