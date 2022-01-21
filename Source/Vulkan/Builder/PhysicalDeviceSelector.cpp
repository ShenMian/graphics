// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "PhysicalDeviceSelector.h"
#include <stdexcept>

PhysicalDeviceSelector::PhysicalDeviceSelector(const VKInstance& instance, VkSurfaceKHR surface)
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

		if(!(device.getProperties().deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
			device.getFeatures().geometryShader))
			continue;

		if(info.requireGraphicsQueue)
			if(device.graphics == -1)
				continue;

		if(info.requireComputeQueue)
			if(device.compute == -1)
				continue;

		if(info.requireTransferQueue)
			if(device.transfer == -1)
				continue;

		if(info.requirePresentQueue)
			if(device.present == -1)
				continue;

		if(!device.isExtensionAvailable(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
			continue;

		return device;
	}
	throw std::runtime_error("failed to select suitable physical device");
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireGraphicsQueue()
{
	info.requireGraphicsQueue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireComputeQueue()
{
	info.requireComputeQueue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requireTransferQueue()
{
	info.requireTransferQueue = true;
	return *this;
}

PhysicalDeviceSelector& PhysicalDeviceSelector::requirePresentQueue()
{
	info.requirePresentQueue = true;
	return *this;
}
