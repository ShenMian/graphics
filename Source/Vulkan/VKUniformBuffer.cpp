// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKUniformBuffer.h"
#include <cstring>

VKUniformBuffer::VKUniformBuffer(const std::string& name, int slot, size_t size)
	: UniformBuffer(name, slot),
	buffer(size, Buffer::Type::Uniform,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

void VKUniformBuffer::write(const void* data, size_t size, size_t offset)
{
	std::memcpy(buffer.getData(), reinterpret_cast<const uint8_t*>(data) + offset, size);
}
