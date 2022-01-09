// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKUniformBuffer.h"
#include <cstring>

VKUniformBuffer::VKUniformBuffer(size_t size)
	: buffer(size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

void VKUniformBuffer::write(const void* data, size_t size, size_t offset)
{
	std::memcpy(buffer.getMappedMemory(), reinterpret_cast<const uint8_t*>(data) + offset, size);
}
