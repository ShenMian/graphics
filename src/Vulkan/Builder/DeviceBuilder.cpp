// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "DeviceBuilder.h"
#include <set>
#include <stdexcept>

DeviceBuilder::DeviceBuilder(VKPhysicalDevice& physicalDevice) : physical_device_(physicalDevice)
{
	if(!physicalDevice.is_extension_available(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		throw std::runtime_error("physical device do not support swapchain extension");
}

VKDevice DeviceBuilder::build()
{
	VkPhysicalDeviceFeatures deviceFeatures = {};

	std::set<uint32_t> uniqueQueueIndices;
	if(physical_device_.graphics.has_value())
		uniqueQueueIndices.insert(physical_device_.graphics.value());
	if(physical_device_.compute.has_value())
		uniqueQueueIndices.insert(physical_device_.compute.value());
	if(physical_device_.transfer.has_value())
		uniqueQueueIndices.insert(physical_device_.transfer.value());
	if(physical_device_.present.has_value())
		uniqueQueueIndices.insert(physical_device_.present.value());

	float queuePriority = 1.0f;
	for(const auto index : uniqueQueueIndices)
	{
		VkDeviceQueueCreateInfo info = {};
		info.sType                   = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.queueFamilyIndex        = index;
		info.queueCount              = 1;
		info.pQueuePriorities        = &queuePriority;
		queue_infos_.emplace_back(info);
	}

	std::vector<const char*> layers;

	std::vector<const char*> extensions;
	extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

	VkDeviceCreateInfo deviceInfo      = {};
	deviceInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount    = static_cast<uint32_t>(queue_infos_.size());
	deviceInfo.pQueueCreateInfos       = queue_infos_.data();
	deviceInfo.enabledLayerCount       = static_cast<uint32_t>(layers.size());
	deviceInfo.ppEnabledLayerNames     = layers.data();
	deviceInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
	deviceInfo.ppEnabledExtensionNames = extensions.data();
	deviceInfo.pEnabledFeatures        = &deviceFeatures;

	VkDevice device;
	if(vkCreateDevice(physical_device_, &deviceInfo, nullptr, &device) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device");

	return {device, physical_device_};
}
