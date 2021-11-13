// Copyright 2021 SMS
// License(Apache-2.0)

#include "Graphics.h"

struct Vertex
{
	Vector2 position;
	Vector3 color;
};

int main()
{
	Window::init();

	Window window("Example", {960, 540}); // 创建一个标题为 Example, 大小为 960x540 像素的窗口 (并初始化 glad)

	// 打印基本信息
	auto renderer = Renderer::get();
	puts(std::format("Device:   {}", renderer->getDeviceName()).c_str());
	puts(std::format("Renderer: {}", renderer->getRendererName()).c_str());
	puts(std::format("Vendor:   {}", renderer->getVendorName()).c_str());

	auto forword = Program::create("Shaders/forword"); // 从源文件创建着色器程序

	Model model;
	model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj");
	// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx"); // 暂不支持格式的压缩纹理资源
	// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf");

	window.setVisible(true); // 设置窗口可见

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		forword->use();
		cmdBuffer->begin();
		{
			cmdBuffer->setClearColor({0, 0, 0, 0});
			cmdBuffer->clear(ClearFlag::Color);

			for(const auto& mesh : model.getMeshs())
			{
				const auto vbo = mesh.getVertexBuffer();
				const auto ibo = mesh.getIndexBuffer();

				cmdBuffer->setVertexBuffer(vbo);
				cmdBuffer->setIndexBuffer(ibo);
				cmdBuffer->draw(0, vbo->getCount());
			}
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	return 0;
}