// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Monitor.h"
#include <GLFW/glfw3.h>

#include <optional>

std::vector<Monitor> Monitor::monitors_;

const Monitor* Monitor::get_primary() noexcept
{
	const auto it = std::find(monitors_.begin(), monitors_.end(), Monitor(glfwGetPrimaryMonitor()));
	if(it == monitors_.end())
		return nullptr;
	return &(*it);
}

const std::vector<Monitor>& Monitor::get_monitors()
{
	return monitors_;
}

std::string_view Monitor::get_name() const
{
	return glfwGetMonitorName(handle_);
}

Vector2i Monitor::get_size() const
{
	const auto videoMode = glfwGetVideoMode(handle_);
	return {videoMode->width, videoMode->height};
}

Vector2i Monitor::get_position() const
{
	int x, y;
	glfwGetMonitorPos(handle_, &x, &y);
	return {x, y};
}

int Monitor::get_refresh_rate() const
{
	const auto videoMode = glfwGetVideoMode(handle_);
	return videoMode->refreshRate;
}

bool Monitor::is_primary() const
{
	return handle_ == glfwGetPrimaryMonitor();
}

GLFWmonitor* Monitor::get_handle() const
{
	return handle_;
}

Monitor::Monitor(GLFWmonitor* handle) : handle_(handle)
{
}

void Monitor::init()
{
	static auto update = [] {
		int        count;
		const auto handles = glfwGetMonitors(&count);
		monitors_.clear();
		for(int i = 0; i < count; i++)
			monitors_.push_back(std::move(Monitor(handles[i])));
		if(monitors_.empty())
			throw std::runtime_error("do no have any monitor");
	};

	glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) { update(); });
	update();
}

void Monitor::deinit()
{
	glfwSetMonitorCallback(nullptr);
}
