// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKPhysicalDevice.h"
#include <vulkan/vulkan.h>

class VKDevice final
{
public:
	enum class QueueType
	{
		Graphics,
		Compute,
		Transfer,
		Present
	};

	VKDevice(VkDevice device, VKPhysicalDevice physicalDevice);
	~VKDevice();

	VkQueue getQueue(QueueType type) const;
	uint32_t getQueueIndex(QueueType type) const;

	operator VkDevice() const;

private:
	VkDevice         handle;
	VKPhysicalDevice physicalDevice;
	VkSurfaceKHR     surface;
};
