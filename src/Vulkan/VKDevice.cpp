// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKDevice.h"
#include <stdexcept>

VKDevice::VKDevice(VkDevice device) : handle_(device)
{
}

VKDevice::VKDevice(VkDevice device, VKPhysicalDevice& physicalDevice) : handle_(device), physical_device_(physicalDevice)
{
}

VkQueue VKDevice::get_queue(QueueType type) const
{
	const auto index = get_queue_index(type);
	if(!index.has_value())
		throw std::runtime_error("requested queue do not exist");

	VkQueue queue;
	vkGetDeviceQueue(handle_, index.value(), 0, &queue);
	return queue;
}

VKDevice::operator VkDevice() noexcept
{
	return handle_;
}

VKDevice::operator VkDevice() const noexcept
{
	return handle_;
}

std::optional<uint32_t> VKDevice::get_queue_index(QueueType type) const
{
	switch(type)
	{
		using enum QueueType;

	case Graphics:
		return physical_device_.graphics;

	case Compute:
		return physical_device_.compute;

	case Transfer:
		return physical_device_.transfer;

	case Present:
		return physical_device_.present;
	}
	return std::nullopt;
}

VKPhysicalDevice& VKDevice::get_physical_device() noexcept
{
	return physical_device_;
}

void VKDevice::destroy()
{
	vkDestroyDevice(handle_, nullptr);
}
