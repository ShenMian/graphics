// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "PhysicalDeviceSelector.h"
#include <stdexcept>

PhysicalDeviceSelector::PhysicalDeviceSelector(VKInstance& instance, VkSurfaceKHR surface)
{
	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
	if(deviceCount == 0)
		throw std::runtime_error("failed to find GPUs with Vulkan support");
	std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data());

	for(auto& device : physicalDevices)
		devices_.emplace_back(device, surface);
}

VKPhysicalDevice PhysicalDeviceSelector::select()
{
	for(const auto& device : devices_)
	{
		// TODO: 需要兼容没有独显的环境
		if(!(device.get_properties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		     device.get_features().geometryShader))
			continue;

		if(info_.require_graphics_queue)
			if(!device.graphics.has_value())
				continue;

		if(info_.require_compute_queue)
			if(!device.compute.has_value())
				continue;

		if(info_.require_transfer_queue)
			if(!device.transfer.has_value())
				continue;

		if(info_.require_present_queue)
			if(!device.present.has_value())
				continue;

		if(info_.require_multi_viewport)
			if(!device.get_features().multiViewport)
				continue;

		if(!device.is_extension_available(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
			continue;

		return device;
	}
	throw std::runtime_error("failed to select suitable physical device");
}

PhysicalDeviceSelector& PhysicalDeviceSelector::require_graphics_queue() noexcept
{
	info_.require_graphics_queue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::require_compute_queue() noexcept
{
	info_.require_compute_queue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::require_transfer_queue() noexcept
{
	info_.require_transfer_queue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::require_present_queue() noexcept
{
	info_.require_present_queue = true;
	return *this;
}
