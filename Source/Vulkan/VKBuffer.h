// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include "VKRenderer.h"
#include <vulkan/vulkan.h>

class VKBuffer : public Buffer
{
public:
	VKBuffer(size_t size, Buffer::Type type, Buffer::Usage usage, VkMemoryPropertyFlags properties);
	virtual ~VKBuffer();

	void map(size_t size = VK_WHOLE_SIZE, size_t offset = 0) override;
	void unmap() override;
	void flush(size_t size = VK_WHOLE_SIZE, size_t offset = 0) override;

	operator VkBuffer();
	operator VkBuffer() const;

private:
	uint32_t findMemoryType(uint32_t type, VkMemoryPropertyFlags properties);

	VkBuffer       handle;
	VkDeviceMemory memory;

	VKRenderer* renderer;
};
