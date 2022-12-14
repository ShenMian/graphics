// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "IndexBuffer.h"
#include "Renderer.h"
#include <cstring>

#include "OpenGL/GLIndexBuffer.h"
#include "Vulkan/VKIndexBuffer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::create(std::span<const uint32_t> data, Buffer::Usage usage)
{
	return create(data.data(), data.size_bytes(), usage);
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* data, size_t size, Buffer::Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLIndexBuffer>(data, size, usage);

	case Vulkan:
		return std::make_shared<VKIndexBuffer>(data, size, usage);
	}
	return nullptr;
}

size_t IndexBuffer::getSize() const
{
	return size;
}

uint32_t IndexBuffer::getCount() const
{
	return count;
}

IndexBuffer::IndexBuffer(const void* data, size_t size)
    : size(size), count(static_cast<uint32_t>(size / sizeof(unsigned int)))
{
}
