// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLUniformBuffer.h"
#include "GLProgram.h"
#include "GLCheck.h"
#include <stdexcept>

#include <string>

GLUniformBuffer::GLUniformBuffer(std::string_view name, int binding, size_t size)
	: UniformBuffer(name, binding), buffer(size, Buffer::Type::Uniform, Buffer::Usage::Dynamic)
{
}

void GLUniformBuffer::map(size_t size, size_t offset)
{
	buffer.map(size, offset);
}

void GLUniformBuffer::unmap()
{
	buffer.unmap();
}

void GLUniformBuffer::write(const void* data, size_t size, size_t offset)
{
	buffer.write(data, size, offset);
}

void GLUniformBuffer::bind(GLProgram* program)
{
	// FIXME: SPRI-V Shader无法再正确识别该项
	auto blockIndex = glGetUniformBlockIndex(*program, name.c_str());
	blockIndex = 0; // FIXME
	if(blockIndex == GL_INVALID_INDEX)
		throw std::runtime_error("failed to get uniform block index");
	glUniformBlockBinding(*program, blockIndex, binding);
	glBindBufferBase(GL_UNIFORM_BUFFER, blockIndex, buffer);
	GLCheckError();
}
