// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLBuffer.h"
#include "GLCheck.h"
#include <cstring>

GLBuffer::GLBuffer(size_t size, GLenum type)
	: size(size), type(type)
{
	glCreateBuffers(1, &handle);
	allocate(size);
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLBuffer::map(size_t size, size_t offset)
{
	const GLenum access = GL_READ_WRITE;
	if(size == -1)
		size = this->size;

	bind();
	data = glMapBufferRange(type, offset, size, access);
	GLCheckError();
}

void GLBuffer::unmap()
{
	bind();
	glUnmapBuffer(type);
	data = nullptr;
	GLCheckError();
}

void GLBuffer::write(const void* data, size_t size, size_t offset)
{
	std::memcpy(static_cast<unsigned char*>(this->data) + offset, data, size);
}

void GLBuffer::allocate(size_t size)
{
	bind();
	glBufferData(type, size, nullptr, GL_DYNAMIC_DRAW);
	GLCheckError();
}

void GLBuffer::bind()
{
	glBindBuffer(type, handle);
}

GLBuffer::operator GLuint() const
{
	return handle;
}
