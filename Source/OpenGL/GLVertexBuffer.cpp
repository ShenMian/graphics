// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLVertexBuffer.h"
#include "GLCheck.h"
#include <cstring>
#include <unordered_map>

namespace
{

std::unordered_map<Buffer::Usage, uint32_t> GLUsage = {
	{Buffer::Usage::Static, GL_STATIC_DRAW},
	{Buffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
	{Buffer::Usage::Stream, GL_STREAM_DRAW}
};

}

GLVertexBuffer::GLVertexBuffer(const void* data, size_t size, const VertexAttributes& fmt, Buffer::Usage usage)
	: VertexBuffer(data, size, fmt), buffer(size, Buffer::Type::Vertex, usage)
{
    write(data, size);
	vao.build(fmt);
}

void GLVertexBuffer::map()
{
	buffer.map();
}

void GLVertexBuffer::unmap()
{
	buffer.unmap();
}

void GLVertexBuffer::write(const void* data, size_t size)
{
	buffer.map();
	buffer.write(data, size);
	buffer.unmap();
}

void GLVertexBuffer::bind()
{
	buffer.bind();
	vao.bind();
}
