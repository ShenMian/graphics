// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "IndexBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLIndexBuffer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::create(const void* data, size_t size, size_t count, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLIndexBuffer>(data, size, count, usage);
	}
	return nullptr;
}

size_t IndexBuffer::getSize() const
{
	return size;
}

size_t IndexBuffer::getCount() const
{
	return count;
}

IndexBuffer::IndexBuffer(size_t size, size_t count)
	: size(size), count(count)
{
}
