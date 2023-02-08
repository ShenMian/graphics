// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKIndexBuffer.h"

VKIndexBuffer::VKIndexBuffer(const uint32_t* data, size_t size, Buffer::Usage usage)
    : IndexBuffer(data, size), buffer(size, Buffer::Type::Index, usage,
                                      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
	buffer.map();
	buffer.write(data, size);
	buffer.unmap();
}

Buffer& VKIndexBuffer::getBuffer()
{
	return buffer;
}

VKIndexBuffer::operator VkBuffer()
{
	return buffer;
}

VKIndexBuffer::operator VkBuffer() const
{
	return buffer;
}
