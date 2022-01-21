// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <vulkan/vulkan.h>

class VKPhysicalDevice final
{
public:
	VKPhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface);

	uint32_t graphics = -1;
	uint32_t compute = -1;
	uint32_t transfer = -1;
	uint32_t present = -1;

	operator VkPhysicalDevice() const;

private:
	VkPhysicalDevice handle;
};
