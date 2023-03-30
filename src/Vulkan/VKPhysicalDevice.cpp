// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKPhysicalDevice.h"
#include <algorithm>
#include <stdexcept>
#include <vector>

VKPhysicalDevice::VKPhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface) : handle_(device), surface_(surface)
{
	vkGetPhysicalDeviceProperties(device, &properties_);
	vkGetPhysicalDeviceFeatures(device, &features_);

	// 获取队列族索引
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(handle_, &queueFamilyCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(handle_, &queueFamilyCount, queueFamilies.data());
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
	vkEnumerateDeviceExtensionProperties(handle_, nullptr, &extensionCount, nullptr);
	available_extensions_.resize(extensionCount);
	vkEnumerateDeviceExtensionProperties(handle_, nullptr, &extensionCount, available_extensions_.data());
}

const VkPhysicalDeviceProperties& VKPhysicalDevice::get_properties() const noexcept
{
	return properties_;
}

const VkPhysicalDeviceFeatures& VKPhysicalDevice::get_features() const noexcept
{
	return features_;
}

VkSurfaceKHR VKPhysicalDevice::get_surface() const noexcept
{
	return surface_;
}

std::string_view VKPhysicalDevice::get_name() const noexcept
{
	return properties_.deviceName;
}

std::string_view VKPhysicalDevice::get_vendor_name() const noexcept
{
	switch(properties_.vendorID)
	{
	case 0x1002:
		return "Advanced Micro Devices, Inc.";
	case 0x10de:
		return "NVIDIA Corporation";
	case 0x102b:
		return "Matrox Electronic Systems Ltd.";
	case 0x1414:
		return "Microsoft Corporation";
	case 0x5333:
		return "S3 Graphics Co., Ltd.";
	case 0x8086:
		return "Intel Corporation";
	case 0x80ee:
		return "Oracle Corporation";
	case 0x15ad:
		return "VMware Inc.";
	}
	return "Unknown";
}

VkSurfaceCapabilitiesKHR VKPhysicalDevice::get_surface_capabilities() const
{
	VkSurfaceCapabilitiesKHR capabilities;
	if(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(handle_, surface_, &capabilities) != VK_SUCCESS)
		throw std::runtime_error("failed to get surface capabilities");
	return capabilities;
}

std::vector<VkSurfaceFormatKHR> VKPhysicalDevice::get_surface_formats() const
{
	std::vector<VkSurfaceFormatKHR> formats;
	uint32_t                        formatCount;
	if(vkGetPhysicalDeviceSurfaceFormatsKHR(handle_, surface_, &formatCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error("failed to get surface formats");
	formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(handle_, surface_, &formatCount, formats.data());
	return formats;
}

std::vector<VkPresentModeKHR> VKPhysicalDevice::get_surface_present_modes() const
{
	std::vector<VkPresentModeKHR> presentModes;
	uint32_t                      presentModeCount;
	if(vkGetPhysicalDeviceSurfacePresentModesKHR(handle_, surface_, &presentModeCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error("failed to get surface present modes");
	presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(handle_, surface_, &presentModeCount, presentModes.data());
	return presentModes;
}

bool VKPhysicalDevice::is_extension_available(std::string_view name) const
{
	return std::ranges::any_of(available_extensions_, [name](const auto& ext) { return ext.extensionName == name; });
}

VKPhysicalDevice::operator VkPhysicalDevice()
{
	return handle_;
}

VKPhysicalDevice::operator VkPhysicalDevice() const
{
	return handle_;
}
