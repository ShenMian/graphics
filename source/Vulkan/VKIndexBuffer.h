// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "IndexBuffer.h"
#include "VKBuffer.h"
#include <vulkan/vulkan.h>

class VKIndexBuffer : public IndexBuffer
{
public:
	VKIndexBuffer(const uint32_t* data, size_t size, Buffer::Usage usage);

	Buffer& getBuffer() override;

	operator VkBuffer();
	operator VkBuffer() const;

private:
	VKBuffer buffer;
};
