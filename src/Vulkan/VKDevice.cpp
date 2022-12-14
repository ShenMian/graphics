// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKDevice.h"
#include <stdexcept>

VKDevice::VKDevice(VkDevice device) : handle(device)
{
}

VKDevice::VKDevice(VkDevice device, VKPhysicalDevice& physicalDevice) : handle(device), physicalDevice(physicalDevice)
{
}

VkQueue VKDevice::getQueue(QueueType type) const
{
	const auto index = getQueueIndex(type);
	if(!index.has_value())
		throw std::runtime_error("requested queue do not exist");

	VkQueue queue;
	vkGetDeviceQueue(handle, index.value(), 0, &queue);
	return queue;
}

VKDevice::operator VkDevice() noexcept
{
	return handle;
}

VKDevice::operator VkDevice() const noexcept
{
	return handle;
}

std::optional<uint32_t> VKDevice::getQueueIndex(QueueType type) const
{
	switch(type)
	{
		using enum QueueType;

	case Graphics:
		return physicalDevice.graphics;

	case Compute:
		return physicalDevice.compute;

	case Transfer:
		return physicalDevice.transfer;

	case Present:
		return physicalDevice.present;
	}
	return std::nullopt;
}

VKPhysicalDevice& VKDevice::getPhysicalDevice() noexcept
{
	return physicalDevice;
}

void VKDevice::destroy()
{
	vkDestroyDevice(handle, nullptr);
}
