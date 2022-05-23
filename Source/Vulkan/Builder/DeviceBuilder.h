// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKPhysicalDevice.h"
#include "../VKDevice.h"
#include <vulkan/vulkan.h>
#include <vector>

class DeviceBuilder final
{
public:
	DeviceBuilder(VKPhysicalDevice& physcialDevice);

	VKDevice build();

private:
	std::vector<VkDeviceQueueCreateInfo> queueInfos;

	VKPhysicalDevice& physicalDevice;
};
