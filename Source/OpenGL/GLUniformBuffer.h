// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <cstdint>
#include <glad/glad.h>

class GLUniformBuffer
{
public:
	GLUniformBuffer(size_t size);
	virtual ~GLUniformBuffer();

	void write(const void* data, size_t size, size_t offset = 0);

	void bind();

private:
	GLuint handle;
	size_t size;
};
