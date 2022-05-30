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
	GLUniformBuffer(std::string_view name, int binding, size_t size);
	
	Buffer& getBuffer() override;

	void bind(GLProgram* program);

private:
	GLBuffer buffer;
};
