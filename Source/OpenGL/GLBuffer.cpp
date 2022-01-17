// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLBuffer.h"
#include "GLCheck.h"
#include <cassert>
#include <cstring>
#include <unordered_map>

namespace
{

std::unordered_map<Buffer::Type, uint32_t> GLType = {
	{Buffer::Type::Vertex, GL_ARRAY_BUFFER},
	{Buffer::Type::Index, GL_ELEMENT_ARRAY_BUFFER},
	{Buffer::Type::Uniform, GL_UNIFORM_BUFFER}
};

std::unordered_map<Buffer::Usage, uint32_t> GLUsage = {
	{Buffer::Usage::Static, GL_STATIC_DRAW},
	{Buffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
	{Buffer::Usage::Stream, GL_STREAM_DRAW}
};

}

GLBuffer::GLBuffer(size_t size, Type type, Usage usage)
	: Buffer(size, type, usage), glType(GLType[type])
{
	glCreateBuffers(1, &handle);
	bind();
	glBufferData(glType, size, nullptr, GLUsage[usage]);
	GLCheckError();
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLBuffer::map(size_t size, size_t offset)
{
	const GLenum access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
	if(data)
		return; // 缓冲区已处于映射状态
	if(size == -1)
		size = this->size;
	assert(size <= this->size);

	bind();
	data = glMapBufferRange(glType, offset, size, access);
	GLCheckError();
}

void GLBuffer::unmap()
{
	bind();
	glUnmapBuffer(glType);
	data = nullptr;
	GLCheckError();
}

void GLBuffer::write(const void* data, size_t size, size_t offset)
{
	std::memcpy(static_cast<unsigned char*>(this->data) + offset, data, size);
	// glBufferSubData(glType, offset, size, data);
}

void GLBuffer::bind()
{
	glBindBuffer(glType, handle);
}

GLBuffer::operator GLuint() const
{
	return handle;
}
