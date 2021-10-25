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

	{
		Timer timer;

		for(int i = 0; i < 5; i++)
		{
			std::vector<Model> models(4);
			models[0].load("../../3DModel/weapon/m4a1/m4a1.gltf");
			models[1].load("../../3DModel/weapon/m1911/m1911.gltf");
			models[2].load("../../3DModel/scene/SunTemple/SunTemple.fbx");
			models[3].load("../../3DModel/scene/Crytek_Sponza/sponza.obj");
		}

		puts(std::format("同步加载: {} ms", timer.getMilliseconds()).c_str());
	}

	{
		Timer timer;

		for(int i = 0; i < 5; i++)
		{
			std::atomic<int> t = 4;
			std::vector<Model> models(4);
			models[0].loadAsync("../../3DModel/weapon/m4a1/m4a1.gltf", [&t](std::error_code ec)
			{
				t--;
			});
			models[1].loadAsync("../../3DModel/weapon/m1911/m1911.gltf", [&t](std::error_code ec)
			{
				t--;
			});
			models[2].loadAsync("../../3DModel/scene/SunTemple/SunTemple.fbx", [&t](std::error_code ec)
			{
				t--;
			});
			models[3].loadAsync("../../3DModel/scene/Crytek_Sponza/sponza.obj", [&t](std::error_code ec)
			{
				t--;
			});
			while(t != 0)
				;
		}

		puts(std::format("异步加载: {} ms", timer.getMilliseconds()).c_str());
	}

	while(true)
		window.update();

	return 0;
}

// "../../3DModel/scene/SunTemple/SunTemple.fbx"