// Copyright 2021 SMS
// License(Apache-2.0)

#include "Graphics.h"

#include <chrono>
#include <atomic>

class Timer
{
public:
	Timer()
		: start(std::chrono::high_resolution_clock::now())
	{
	}

	auto getMilliseconds() const
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}

private:
	std::chrono::steady_clock::time_point start;
};

int main()
{
	Window::init();

	Window window("Title", {1920 / 2, 1080 / 2});

	/*
	Model model;
	model.loadAsync("../../3DModel/scene/SunTemple/SunTemple.fbx", [](std::error_code ec)
	{
		if(ec)
			return;
		puts("加载完毕.");
	});*/

	bool running = true;

	window.onClose = [&]() { running = false; };

	auto cmds = CommandBuffer::create();

	while(running)
	{
		cmds->begin();
		{
			cmds->setViewport({0, 0}, {800, 200});
			cmds->setClearColor({0, 1, 0, 0});
			cmds->clear(ClearFlag::Color | ClearFlag::Depth);
		}
		cmds->end();
		cmds->execute();

		window.update();
	}

	return 0;
}