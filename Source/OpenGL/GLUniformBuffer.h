// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "UniformBuffer.h"
#include "GLBuffer.h"
#include <glad/glad.h>
#include <cstddef>

class GLProgram;

class GLUniformBuffer : public UniformBuffer
{
public:
	GLUniformBuffer(const std::string& name, int binding, size_t size);

	void write(const void* data, size_t size, size_t offset = 0);

	void bind(GLProgram* program);
	void bind();

private:
	GLBuffer buffer;
};
