// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKUniformBuffer.h"
#include <cstring>

VKUniformBuffer::VKUniformBuffer(int binding, size_t size)
    : UniformBuffer(binding), buffer_(size, Buffer::Type::Uniform, Buffer::Usage::Dynamic,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
{
}

Buffer& VKUniformBuffer::get_buffer()
{
	return buffer_;
}
