// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKDevice.h"
#include "../VKPhysicalDevice.h"
#include <vector>
#include <vulkan/vulkan.h>

class DeviceBuilder final
{
public:
	DeviceBuilder(VKPhysicalDevice& physcialDevice);

	VKDevice build();

private:
	std::vector<VkDeviceQueueCreateInfo> queueInfos;

	VKPhysicalDevice& physicalDevice;
};
