// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Monitor.h"
#include <glfw/glfw3.h>

Monitor              Monitor::primary;
std::vector<Monitor> Monitor::monitors;

const Monitor& Monitor::getPrimary()
{
    return primary;
}

const std::vector<Monitor>& Monitor::getMonitors()
{
    return monitors;
}

const std::string& Monitor::getName() const
{
    return name;
}

Vector2i Monitor::getSize() const
{
    const auto videoMode = glfwGetVideoMode(handle);
    return {videoMode->width, videoMode->height};
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

void* Monitor::getNativeHandle() const
{
    return handle;
}

Monitor::Monitor(GLFWmonitor* handle)
    : handle(handle), name(glfwGetMonitorName(handle))
{
}

void Monitor::init()
{
	glfwSetMonitorCallback([](GLFWmonitor* monitor, int event) { update(); });
    update();
}

void Monitor::deinit()
{
    glfwSetMonitorCallback(nullptr);
}

void Monitor::update()
{
    primary = Monitor(glfwGetPrimaryMonitor());

    int count;
    const auto handles = glfwGetMonitors(&count);
    monitors.clear();
    for(int i = 0; i < count; i++)
        monitors.push_back(std::move(Monitor(handles[i])));
}
