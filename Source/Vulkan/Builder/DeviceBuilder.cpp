// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "DeviceBuilder.h"
#include <set>
#include <stdexcept>

DeviceBuilder::DeviceBuilder(VKPhysicalDevice& physicalDevice)
	: physicalDevice(physicalDevice)
{
}

VKDevice DeviceBuilder::build()
{
	VkPhysicalDeviceFeatures deviceFeatures = {};

	std::set<uint32_t> uniqueQueueIndices;
	if(physicalDevice.graphics != -1)
		uniqueQueueIndices.insert(physicalDevice.graphics);
	if(physicalDevice.compute != -1)
		uniqueQueueIndices.insert(physicalDevice.compute);
	if(physicalDevice.transfer != -1)
		uniqueQueueIndices.insert(physicalDevice.transfer);
	if(physicalDevice.present != -1)
		uniqueQueueIndices.insert(physicalDevice.present);

	float queuePriority = 1.0f;
	for(auto index : uniqueQueueIndices)
	{
		VkDeviceQueueCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		info.queueFamilyIndex = index;
		info.queueCount = 1;
		info.pQueuePriorities = &queuePriority;
		queueInfos.emplace_back(info);
	}

	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = static_cast<uint32_t>(queueInfos.size());
	deviceInfo.pQueueCreateInfos = queueInfos.data();
	deviceInfo.pEnabledFeatures = &deviceFeatures;

	VkDevice device;
	if(vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device) != VK_SUCCESS)
		throw std::runtime_error("failed to create logical device");

	return device;
}
