// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "GLBuffer.h"
#include "IndexBuffer.h"
#include <glad/glad.h>

class GLIndexBuffer : public IndexBuffer
{
public:
	GLIndexBuffer(const uint32_t* data, size_t size, Buffer::Usage usage);

	Buffer& get_buffer() override;

	void bind();

private:
	GLBuffer buffer;
};
