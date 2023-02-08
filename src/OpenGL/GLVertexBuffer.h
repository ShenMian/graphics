// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "GLBuffer.h"
#include "GLVertexArray.h"
#include "VertexBuffer.h"
#include <glad/glad.h>

class GLVertexBuffer : public VertexBuffer
{
public:
	GLVertexBuffer(const void* data, size_t size, const VertexFormat& fmt, Buffer::Usage usage);

	Buffer& getBuffer() override;

	void bind();

private:
	GLBuffer      buffer;
	GLVertexArray vao;
};
