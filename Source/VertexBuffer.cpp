// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VertexBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLVertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* data, size_t size, size_t count, const VertexFormat& fmt, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLVertexBuffer>(data, size, count, fmt, usage);
	}
	return nullptr;
}

size_t VertexBuffer::getSize() const
{
	return size;
}

size_t VertexBuffer::getCount() const
{
	return count;
}

const VertexFormat& VertexBuffer::getFormat() const
{
	return format;
}

VertexBuffer::VertexBuffer(size_t size, size_t count, const VertexFormat& fmt)
	: size(size), count(count), format(fmt)
{
}
