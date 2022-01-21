// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKPhysicalDevice.h"
#include <vulkan/vulkan.h>

class VKDevice final
{
public:
	/**
	 * @brief 队列类型.
	 */
	enum class QueueType
	{
		Graphics,
		Compute,
		Transfer,
		Present
	};

	VKDevice(VkDevice device, VKPhysicalDevice physicalDevice);
	~VKDevice();

	/**
	 * @brief 获取队列.
	 *
	 * @param type 队列类型.
	 */
	VkQueue getQueue(QueueType type) const;

	/**
	 * @brief 获取队列索引.
	 *
	 * @param type 队列类型.
	 */
	uint32_t getQueueIndex(QueueType type) const;

	operator VkDevice() const;

private:
	VkDevice         handle;
	VKPhysicalDevice physicalDevice;
	VkSurfaceKHR     surface;
};
