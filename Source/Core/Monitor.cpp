// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Monitor.h"
#include <GLFW/glfw3.h>

#include <optional>

std::vector<Monitor> Monitor::monitors;

const Monitor* Monitor::getPrimary() noexcept
{
	const auto it = std::find(monitors.begin(), monitors.end(), Monitor(glfwGetPrimaryMonitor()));
	if(it == monitors.end())
		return nullptr;
	return &(*it);
}

const std::vector<Monitor>& Monitor::getMonitors()
{
	return monitors;
}

std::string_view Monitor::getName() const
{
	return glfwGetMonitorName(handle);
}

Vector2i Monitor::getSize() const
{
	const auto videoMode = glfwGetVideoMode(handle);
	return {videoMode->width, videoMode->height};
}

Vector2i Monitor::getPosition() const
{
	int x, y;
	glfwGetMonitorPos(handle, &x, &y);
	return {x, y};
}

int Monitor::getRefreshRate() const
{
	const auto videoMode = glfwGetVideoMode(handle);
	return videoMode->refreshRate;
}

bool Monitor::isPrimary() const
{
	return handle == glfwGetPrimaryMonitor();
}

GLFWmonitor* Monitor::getHandle() const
{
	return handle;
}

Monitor::Monitor(GLFWmonitor* handle)
	: handle(handle)
{
}

void Monitor::init()
{
	static auto update = []{
		int count;
		const auto handles = glfwGetMonitors(&count);
		monitors.clear();
		for(int i = 0; i < count; i++)
			monitors.push_back(std::move(Monitor(handles[i])));
		if(monitors.empty())
			throw std::runtime_error("do no have any monitor");
	};
	
	glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) { update(); });
	update();
}

void Monitor::deinit()
{
	glfwSetMonitorCallback(nullptr);
}
