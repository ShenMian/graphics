// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLIndexBuffer.h"
#include "GLCheck.h"
#include <cstring>
#include <unordered_map>

#include "../Renderer.h"

namespace
{

std::unordered_map<IndexBuffer::Usage, uint32_t> GLusage = {
	{IndexBuffer::Usage::Static, GL_STATIC_DRAW},
	{IndexBuffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
	{IndexBuffer::Usage::Stream, GL_STREAM_DRAW}};

}

GLIndexBuffer::GLIndexBuffer(const unsigned int* data, size_t size, Usage usage)
	: IndexBuffer(data, size)
{
	glCreateBuffers(1, &handle);
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GLusage[usage]);
}

GLIndexBuffer::~GLIndexBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLIndexBuffer::map()
{
	bind();
	data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	GLCheckError();
}

void GLIndexBuffer::unmap()
{
	bind();
	glUnmapBuffer(GL_ARRAY_BUFFER);
	data = nullptr;
	GLCheckError();
}

void GLIndexBuffer::write(const void* data, size_t size)
{
	bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GLusage[Usage::Static]);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, data);
	GLCheckError();
}

void GLIndexBuffer::bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle);
}
