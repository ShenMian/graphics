// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKPhysicalDevice.h"
#include <vector>

VKPhysicalDevice::VKPhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
	: handle(device)
{
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, queueFamilies.data());

	for(size_t i = 0; i < queueFamilies.size(); i++)
	{
		if(queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			graphics = i;
		if(queueFamilies[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
			compute = i;
		if(queueFamilies[i].queueFlags & VK_QUEUE_TRANSFER_BIT)
			transfer = i;

		VkBool32 presentSupport;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
		if(presentSupport)
			present = i;
	}
}

VKPhysicalDevice::operator VkPhysicalDevice() const
{
	return handle;
}
