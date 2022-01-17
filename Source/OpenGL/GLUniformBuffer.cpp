// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLUniformBuffer.h"

GLUniformBuffer::GLUniformBuffer(size_t size)
	: buffer(size, Buffer::Type::Uniform, Buffer::Usage::Dynamic)
{
	/*
	auto blockIndex = glGetUniformBlockIndex();
	if(blockIndex == GL_INVALID_INDEX)
		;
	glUniformBlockBinding(program, blockIndex, binding);

	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, buffer);
	*/
}

void GLUniformBuffer::write(const void* data, size_t size, size_t offset)
{
	buffer.map();
	buffer.write(data, size, offset);
	buffer.unmap();
}

void GLUniformBuffer::bind()
{
	buffer.bind();
}
