// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "GLBuffer.h"
#include <cassert>
#include <cstring>
#include <unordered_map>

namespace
{

std::unordered_map<Buffer::Type, uint32_t> GLType = {{Buffer::Type::Vertex, GL_ARRAY_BUFFER},
                                                     {Buffer::Type::Index, GL_ELEMENT_ARRAY_BUFFER},
                                                     {Buffer::Type::Uniform, GL_UNIFORM_BUFFER}};

std::unordered_map<Buffer::Usage, uint32_t> GLUsage = {{Buffer::Usage::Static, GL_STATIC_DRAW},
                                                       {Buffer::Usage::Dynamic, GL_DYNAMIC_DRAW},
                                                       {Buffer::Usage::Stream, GL_STREAM_DRAW}};

} // namespace

GLBuffer::GLBuffer(size_t size, Type type, Usage usage) : Buffer(size, type, usage), glType(GLType[type])
{
	glCreateBuffers(1, &handle);
	glNamedBufferData(handle, size, nullptr, GLUsage[usage]);
}

GLBuffer::~GLBuffer()
{
	glDeleteBuffers(1, &handle);
}

void GLBuffer::map(size_t size, size_t offset)
{
	const GLenum access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT;
	if(data_)
		return; // 缓冲区已处于映射状态
	if(size == (size_t)-1)
		size = this->size_;
	assert(size <= this->size_);

	data_ = glMapNamedBufferRange(handle, offset, size, access);
}

void GLBuffer::unmap()
{
	glUnmapNamedBuffer(handle);
	data_ = nullptr;
}

void GLBuffer::flush(size_t size, size_t offset)
{
	if(size == (size_t)-1)
		size = this->size_;
	assert(size <= this->size_);

	glFlushMappedNamedBufferRange(handle, offset, size);
}

void GLBuffer::bind()
{
	glBindBuffer(glType, handle);
}

GLBuffer::operator GLuint() noexcept
{
	return handle;
}

GLBuffer::operator GLuint() const noexcept
{
	return handle;
}
