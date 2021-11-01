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

	Window window("Example", {1920 / 2, 1080 / 2});
	window.setVisible(true);

	Model model;
	model.loadAsync("../../3DModel/scene/SunTemple/SunTemple.fbx", [](std::error_code ec)
	{
		if(ec) return;
		puts("加载完毕.");
	});

	auto cmdQueue  = CommandQueue::create();
	auto cmdBuffer = CommandBuffer::create();

	bool running   = true;
	window.onClose = [&]() { running = false; };
	while(running)
	{
		cmdBuffer->begin();
		{
			cmdBuffer->setClearColor({0, 0.3, 0, 0});
			cmdBuffer->clear(ClearFlag::Color | ClearFlag::Depth);
		}
		cmdBuffer->end();
		cmdQueue->submit(cmdBuffer);

		window.update();
	}

	return 0;
}