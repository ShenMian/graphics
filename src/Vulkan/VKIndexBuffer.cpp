// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKIndexBuffer.h"

VKIndexBuffer::VKIndexBuffer(const uint32_t* data, size_t size, Buffer::Usage usage)
    : IndexBuffer(data, size), buffer_(size, Buffer::Type::Index, usage,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	buffer_.map();
	buffer_.write(data, size);
	buffer_.unmap();
}

Buffer& VKIndexBuffer::get_buffer()
{
	return buffer_;
}

VKIndexBuffer::operator VkBuffer()
{
	return buffer_;
}

VKIndexBuffer::operator VkBuffer() const
{
	return buffer_;
}
