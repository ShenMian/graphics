// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "GLBuffer.h"
#include "UniformBuffer.h"
#include <cstddef>
#include <glad/glad.h>

class GLProgram;

class GLUniformBuffer : public UniformBuffer
{
public:
	GLUniformBuffer(int binding, size_t size);

	Buffer& get_buffer() override;

private:
	GLBuffer buffer;
};
