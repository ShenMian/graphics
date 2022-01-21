// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKPhysicalDevice.h"
#include <vector>

VKPhysicalDevice::VKPhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface)
	: handle(device), surface(surface)
{
	vkGetPhysicalDeviceProperties(device, &properties);
	vkGetPhysicalDeviceFeatures(device, &features);

	// 获取队列族索引
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(handle, &queueFamilyCount, queueFamilies.data());
	for(uint32_t i = 0; i < queueFamilies.size(); i++)
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

	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
	availableExtensions.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

	std::vector<VkSurfaceFormatKHR> formats;
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if(formatCount != 0)
	{
		formats.resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());
	}

	std::vector<VkPresentModeKHR> presentModes;
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if(presentModeCount != 0)
	{
		presentModes.resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data());
	}
}

const VkPhysicalDeviceProperties& VKPhysicalDevice::getProperties() const
{
	return properties;
}

const VkPhysicalDeviceFeatures& VKPhysicalDevice::getFeatures() const
{
	return features;
}

bool VKPhysicalDevice::isExtensionAvailable(std::string_view name) const
{
	for(const auto& ext : availableExtensions)
		if(ext.extensionName == name)
			return true;
	return false;
}

VkSurfaceKHR VKPhysicalDevice::getSurface() const
{
	return surface;
}

VKPhysicalDevice::operator VkPhysicalDevice() const
{
	return handle;
}
