// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKDevice.h"

VKDevice::VKDevice(VkDevice device)
	: handle(device)
{
}

VKDevice::VKDevice(VkDevice device, VKPhysicalDevice& physicalDevice)
	: handle(device), physicalDevice(physicalDevice)
{
}

VKDevice::~VKDevice()
{
	vkDestroyDevice(handle, nullptr);
}

VkQueue VKDevice::getQueue(QueueType type) const
{
	VkQueue queue;
	vkGetDeviceQueue(handle, getQueueIndex(type), 0, &queue);
	return VkQueue();
}

VKDevice::operator VkDevice()
{
	return handle;
}

VKDevice::operator VkDevice() const
{
	return handle;
}

uint32_t VKDevice::getQueueIndex(QueueType type) const
{
	uint32_t index = -1;
	switch(type)
	{
		using enum QueueType;

	case Graphics:
		index = physicalDevice.graphics;
		break;

	case Compute:
		index = physicalDevice.compute;
		break;

	case Transfer:
		index = physicalDevice.transfer;
		break;

	case Present:
		index = physicalDevice.present;
		break;
	}
	return index;
}
