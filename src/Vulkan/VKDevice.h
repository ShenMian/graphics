// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKPhysicalDevice.h"
#include <optional>
#include <vulkan/vulkan.h>

/**
 * @brief Vulkan 逻辑设备.
 */
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

	VKDevice() = default;
	VKDevice(VkDevice device);
	VKDevice(VkDevice device, VKPhysicalDevice& physicalDevice);

	/**
	 * @brief 获取队列.
	 *
	 * @param type 队列类型.
	 */
	VkQueue get_queue(QueueType type) const;

	/**
	 * @brief 获取队列索引.
	 *
	 * @param type 队列类型.
	 */
	std::optional<uint32_t> get_queue_index(QueueType type) const;

	VKPhysicalDevice& get_physical_device() noexcept;

	void destroy();

	operator VkDevice() noexcept;
	operator VkDevice() const noexcept;

private:
	VkDevice         handle_;
	VKPhysicalDevice physical_device_;
};
