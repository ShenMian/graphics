// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VertexBuffer.h"
#include <vulkan/vulkan.h>

class VKVertexBuffer : public VertexBuffer
{
public:
	using handle_type = VkBuffer;

	VKVertexBuffer(const void* data, size_t size, uint32_t count, const VertexFormat& fmt, Usage usage);
	virtual ~VKVertexBuffer();

	void bind() override;

private:
	handle_type handle;
};
