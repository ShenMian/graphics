// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKVertexBuffer.h"
#include "VKRenderer.h"
#include <vector>

VKVertexBuffer::VKVertexBuffer(const void* data, size_t size, const VertexAttributes& layout, Buffer::Usage usage)
	: VertexBuffer(data, size, layout),
	buffer(size, Buffer::Type::Vertex, usage,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

void VKVertexBuffer::map()
{
	buffer.map();
}

void VKVertexBuffer::unmap()
{
	buffer.unmap();
}

void VKVertexBuffer::write(const void* data, size_t size)
{
}

void VKVertexBuffer::bind()
{
}

VKVertexBuffer::operator VkBuffer()
{
	return buffer;
}

VKVertexBuffer::operator VkBuffer() const
{
	return buffer;
}
