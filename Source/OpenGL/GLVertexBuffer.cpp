// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLVertexBuffer.h"
#include "GLCheck.h"
#include <cstring>
#include <unordered_map>

namespace
{

std::unordered_map<VertexBuffer::Usage, uint32_t> GLusage = {
	{VertexBuffer::Usage::Static, GL_STATIC_DRAW},
	{VertexBuffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
	{VertexBuffer::Usage::Stream, GL_STREAM_DRAW}};

}

GLVertexBuffer::GLVertexBuffer(const void* data, size_t size, const VertexLayout& fmt, Usage usage)
	: VertexBuffer(data, size, fmt)
{
	glCreateBuffers(1, &handle);
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GLusage[usage]);

	vao.build(fmt);
}

GLVertexBuffer::~GLVertexBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLVertexBuffer::map()
{
	bind();
	data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	GLCheckError();
}

void GLVertexBuffer::unmap()
{
	bind();
	glUnmapBuffer(GL_ARRAY_BUFFER);
	data = nullptr;
	GLCheckError();
}

void GLVertexBuffer::write(const void* data, size_t size)
{
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GLusage[Usage::Static]);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	GLCheckError();
}

void GLVertexBuffer::bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, handle);

	vao.bind();
}
