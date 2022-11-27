// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKInstance.h"
#include "../VKPhysicalDevice.h"
#include <vector>

class VKPhysicalDevice;

class PhysicalDeviceSelector final
{
public:
	PhysicalDeviceSelector(VKInstance& instance, VkSurfaceKHR surface);

	VKPhysicalDevice select();

	PhysicalDeviceSelector& requireGraphicsQueue() noexcept;

	PhysicalDeviceSelector& requireComputeQueue() noexcept;

	PhysicalDeviceSelector& requireTransferQueue() noexcept;

	PhysicalDeviceSelector& requirePresentQueue() noexcept;

private:
	std::vector<VKPhysicalDevice> devices;

	struct
	{
		bool requireGraphicsQueue = false;
		bool requireComputeQueue  = false;
		bool requireTransferQueue = false;
		bool requirePresentQueue  = false;

		bool requireMultiViewport = false;
	} info;
};
