// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include "VKRenderer.h"
#include <vulkan/vulkan.h>

class VKBuffer
{
public:
	VKBuffer(size_t size, Buffer::Type type, VkMemoryPropertyFlags properties);
	virtual ~VKBuffer();

	/**
	 * @brief 映射缓冲区到内存.
	 *
	 * @param size   要映射的大小, 默认值: 全部.
	 * @param offset 偏移量, 单位: byte.
	 * @return VkResult
	 */
	VkResult map(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

	/**
	 * @brief 取消映射.
	 */
	void unmap();

	void* getData();

private:
	uint32_t findMemoryType(uint32_t type, VkMemoryPropertyFlags properties);

	VkBuffer       buffer;
	VkDeviceMemory memory;
	void*          data = nullptr;

	VKRenderer* renderer;
};
