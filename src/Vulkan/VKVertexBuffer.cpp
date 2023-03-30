// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKVertexBuffer.h"
#include "VKRenderer.h"
#include <vector>

VKVertexBuffer::VKVertexBuffer(const void* data, size_t size, const VertexFormat& layout, Buffer::Usage usage)
    : VertexBuffer(data, size, layout),
      buffer_(size, Buffer::Type::Vertex, usage,
             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	buffer_.map();
	buffer_.write(data, size);
	buffer_.unmap();
}

Buffer& VKVertexBuffer::get_buffer()
{
	return buffer_;
}

VKVertexBuffer::operator VkBuffer()
{
	return buffer_;
}

VKVertexBuffer::operator VkBuffer() const
{
	return buffer_;
}
