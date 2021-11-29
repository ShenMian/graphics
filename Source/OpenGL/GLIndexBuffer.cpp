// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLIndexBuffer.h"
#include <unordered_map>

#include "../Renderer.h"

namespace
{

std::unordered_map<IndexBuffer::Usage, uint32_t> GLusage = {
	{IndexBuffer::Usage::Static, GL_STATIC_DRAW},
	{IndexBuffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
	{IndexBuffer::Usage::Stream, GL_STREAM_DRAW}};

}

GLIndexBuffer::GLIndexBuffer(const void* data, size_t size, size_t count, Usage usage)
	: IndexBuffer(size, count)
{
	glCreateBuffers(1, &handle);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GLusage[usage]);
}

GLIndexBuffer::~GLIndexBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLIndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}
