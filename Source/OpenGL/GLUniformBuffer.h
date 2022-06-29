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
	GLUniformBuffer(int binding, size_t size);
	
	Buffer& getBuffer() override;

private:
	GLBuffer buffer;
};
