// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLUniformBuffer.h"
#include "GLProgram.h"
#include "GLCheck.h"
#include <stdexcept>

#include <string>

GLUniformBuffer::GLUniformBuffer(const std::string& name, int slot, size_t size)
	: UniformBuffer(name, slot), buffer(size, Buffer::Type::Uniform, Buffer::Usage::Dynamic)
{
}

void GLUniformBuffer::write(const void* data, size_t size, size_t offset)
{
	buffer.map();
	// buffer.map(size, offset);
	buffer.write(data, size, offset);
	buffer.unmap();
}

void GLUniformBuffer::bind(GLProgram* program)
{
	auto blockIndex = glGetUniformBlockIndex(*program, name.c_str());
	if(blockIndex == GL_INVALID_INDEX)
		throw std::runtime_error("failed to get uniform block index");
	glUniformBlockBinding(*program, blockIndex, slot);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, buffer);
}

void GLUniformBuffer::bind()
{
	buffer.bind();
}
