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

	VkResult map(size_t size = VK_WHOLE_SIZE, size_t offset = 0);
	void unmap();
	void flush(size_t size = VK_WHOLE_SIZE, size_t offset = 0);

	void* getData();

	operator VkBuffer();
	operator VkBuffer() const;

private:
	uint32_t findMemoryType(uint32_t type, VkMemoryPropertyFlags properties);

	VkBuffer       handle;
	VkDeviceMemory memory;
	void* data = nullptr;

	VKRenderer* renderer;
};
