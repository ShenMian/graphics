// Copyright 2021 ShenMian
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

    auto window = new Window("Example", Monitor::getPrimary().getSize() / 2);
	window->setVisible(true); // 设置窗口可见

	auto forword = Program::create("Shaders/forword"); // 从源文件创建着色器程序

	Model model;
	model.load("../../../3DModel/scene/Crytek_Sponza/sponza.obj");
	// model.load("../../../3DModel/scene/SunTemple/SunTemple.fbx"); // 暂不支持格式的压缩纹理资源
	// model.load("../../../3DModel/weapon/m4a1/m4a1.gltf");

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running    = true;
	window->onClose = [&]() { running = false; };
    window->onKey   = [&](int action, Key key)
    {
        if (action == 1)
        {
            switch (key)
            {
            case Key::ESCAPE:
                running = false;
                break;

            case Key::F11:
                window->setFullscreen(!window->isFullscreen());
                break;
            }
        }
    };
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

		window->update();
	}
    delete window;

    Window::deinit();

	return 0;
}
