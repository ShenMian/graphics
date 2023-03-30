// Copyright 2022 ShenMian
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

	PhysicalDeviceSelector& require_graphics_queue() noexcept;

	PhysicalDeviceSelector& require_compute_queue() noexcept;

	PhysicalDeviceSelector& require_transfer_queue() noexcept;

	PhysicalDeviceSelector& require_present_queue() noexcept;

private:
	std::vector<VKPhysicalDevice> devices_;

	struct
	{
		bool require_graphics_queue = false;
		bool require_compute_queue  = false;
		bool require_transfer_queue = false;
		bool require_present_queue  = false;

		bool require_multi_viewport = false;
	} info_;
};
