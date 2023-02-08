// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "GLVertexBuffer.h"
#include <cstring>
#include <unordered_map>

GLVertexBuffer::GLVertexBuffer(const void* data, size_t size, const VertexFormat& fmt, Buffer::Usage usage)
    : VertexBuffer(data, size, fmt), buffer(size, Buffer::Type::Vertex, usage)
{
	buffer.map();
	buffer.write(data, size);
	buffer.unmap();
	
	vao.build(fmt, buffer);
}

Buffer& GLVertexBuffer::getBuffer()
{
	return buffer;
}

void GLVertexBuffer::bind()
{
	// buffer.bind();
	vao.bind();
}
