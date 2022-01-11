// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "IndexBuffer.h"
#include "Renderer.h"
#include <cstring>

#include "OpenGL/GLIndexBuffer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::create(const std::vector<unsigned int>& data, Usage usage)
{
	return create(data.data(), data.size() * sizeof(unsigned int), usage);
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(const unsigned int* data, size_t size, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLIndexBuffer>(data, size, usage);
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

std::vector<uint8_t>& IndexBuffer::getData()
{
	return buffer;
}

const std::vector<uint8_t>& IndexBuffer::getData() const
{
	return buffer;
}

void IndexBuffer::flash()
{
	// map();
	write(buffer.data(), buffer.size());
	// unmap();
}

IndexBuffer::IndexBuffer(const void* data, size_t size)
	: size(size), count(static_cast<uint32_t>(size / sizeof(unsigned int)))
{
	buffer.resize(size);
	std::memcpy(buffer.data(), data, buffer.size());
}
