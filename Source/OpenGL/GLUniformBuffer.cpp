// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLUniformBuffer.h"
#include "GLProgram.h"
#include <stdexcept>

#include <string>

GLUniformBuffer::GLUniformBuffer(int binding, size_t size)
    : UniformBuffer(binding), buffer(size, Buffer::Type::Uniform, Buffer::Usage::Dynamic)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
}

Buffer& GLUniformBuffer::getBuffer()
{
	return buffer;
}

/* FIXME: 使用 SPRI-V 无法再正确识别该项
auto blockIndex = glGetUniformBlockIndex(*program, name.c_str());
if(blockIndex == GL_INVALID_INDEX)
    throw std::runtime_error("failed to get uniform block index");
glUniformBlockBinding(*program, blockIndex, binding);*/
