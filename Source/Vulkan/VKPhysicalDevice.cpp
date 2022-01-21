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

std::string_view VKPhysicalDevice::getName() const
{
	return properties.deviceName;
}

std::string_view VKPhysicalDevice::getVendorName() const
{
	switch(properties.vendorID)
	{
	case 0x1002: return "Advanced Micro Devices, Inc.";
	case 0x10de: return "NVIDIA Corporation";
	case 0x102b: return "Matrox Electronic Systems Ltd.";
	case 0x1414: return "Microsoft Corporation";
	case 0x5333: return "S3 Graphics Co., Ltd.";
	case 0x8086: return "Intel Corporation";
	case 0x80ee: return "Oracle Corporation";
	case 0x15ad: return "VMware Inc.";
	}
	return "Unknown";
}

VKPhysicalDevice::operator VkPhysicalDevice()
{
	return handle;
}

VKPhysicalDevice::operator VkPhysicalDevice() const
{
	return handle;
}
