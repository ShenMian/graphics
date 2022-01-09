// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLUniformBuffer.h"

GLUniformBuffer::GLUniformBuffer(size_t size)
{
	glCreateBuffers(1, &handle);
	bind();
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, handle, 0, size);
}

GLUniformBuffer::~GLUniformBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLUniformBuffer::write(const void* data, size_t size, size_t offset)
{
	bind();
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
}

void GLUniformBuffer::bind()
{
	glBindBuffer(GL_UNIFORM_BUFFER, handle);
}
