// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKPhysicalDevice.h"
#include <vulkan/vulkan.h>
#include <vector>

class DeviceBuilder final
{
public:
	DeviceBuilder(VKPhysicalDevice& physcialDevice);

	VkDevice build();

private:
	std::vector<VkDeviceQueueCreateInfo> queueInfos;
	VkDeviceCreateInfo deviceInfo = {};

	VKPhysicalDevice& physicalDevice;
};
