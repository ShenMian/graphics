// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKBuffer.h"
#include "VertexBuffer.h"
#include <vulkan/vulkan.h>

class VKVertexBuffer : public VertexBuffer
{
public:
	VKVertexBuffer(const void* data, size_t size, const VertexFormat& layout, Buffer::Usage usage);

	Buffer& get_buffer() override;

	operator VkBuffer();
	operator VkBuffer() const;

private:
	VKBuffer buffer_;
};
