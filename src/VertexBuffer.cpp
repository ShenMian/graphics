// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VertexBuffer.h"
#include "Renderer.h"
#include <cstring>

#include "OpenGL/GLVertexBuffer.h"
#include "Vulkan/VKVertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* data, size_t size, const VertexFormat& fmt,
                                                   Buffer::Usage usage)
{
	switch(Renderer::get_backend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		return std::make_shared<GLVertexBuffer>(data, size, fmt, usage);

	case Vulkan:
		return std::make_shared<VKVertexBuffer>(data, size, fmt, usage);
	}
	return nullptr;
}

size_t VertexBuffer::get_size() const
{
	return size_;
}

uint32_t VertexBuffer::get_count() const
{
	return count_;
}

const VertexFormat& VertexBuffer::get_format() const
{
	return format_;
}

VertexBuffer::VertexBuffer(const void* data, size_t size, const VertexFormat& layout)
    : size_(size), count_(static_cast<uint32_t>(size / layout.get_stride())), format_(layout)
{
}
