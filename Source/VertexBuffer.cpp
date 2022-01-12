// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VertexBuffer.h"
#include "Renderer.h"
#include <cstring>

#include "OpenGL/GLVertexBuffer.h"
#include "Vulkan/VKVertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* data, size_t size, const VertexLayout& layout, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLVertexBuffer>(data, size, layout, usage);

	case Vulkan:
		return std::make_shared<VKVertexBuffer>(data, size, layout, usage);
	}
	return nullptr;
}

size_t VertexBuffer::getSize() const
{
	return size;
}

uint32_t VertexBuffer::getCount() const
{
	return count;
}

const VertexLayout& VertexBuffer::getFormat() const
{
	return format;
}

std::vector<uint8_t>& VertexBuffer::getData()
{
	return buffer;
}

const std::vector<uint8_t>& VertexBuffer::getData() const
{
	return buffer;
}

void VertexBuffer::flash()
{
	// map();
	write(buffer.data(), buffer.size());
	// unmap();
}

VertexBuffer::VertexBuffer(const void* data, size_t size, const VertexLayout& layout)
	: size(size), count(static_cast<uint32_t>(size / layout.getStride())), format(layout)
{
	buffer.resize(size);
	std::memcpy(buffer.data(), data, buffer.size());
}
