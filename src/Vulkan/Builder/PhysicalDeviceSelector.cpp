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
		devices.emplace_back(device, surface);
}

VKPhysicalDevice PhysicalDeviceSelector::select()
{
	for(const auto& device : devices)
	{
		// TODO: 需要兼容没有独显的环境
		if(!(device.getProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
		     device.getFeatures().geometryShader))
			continue;

		if(info.requireGraphicsQueue)
			if(!device.graphics.has_value())
				continue;

		if(info.requireComputeQueue)
			if(!device.compute.has_value())
				continue;

		if(info.requireTransferQueue)
			if(!device.transfer.has_value())
				continue;

		if(info.requirePresentQueue)
			if(!device.present.has_value())
				continue;

		if(info.requireMultiViewport)
			if(!device.getFeatures().multiViewport)
				continue;

		if(!device.isExtensionAvailable(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
			continue;

		return device;
	}
	throw std::runtime_error("failed to select suitable physical device");
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireGraphicsQueue() noexcept
{
	info.requireGraphicsQueue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireComputeQueue() noexcept
{
	info.requireComputeQueue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireTransferQueue() noexcept
{
	info.requireTransferQueue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requirePresentQueue() noexcept
{
	info.requirePresentQueue = true;
	return *this;
}
