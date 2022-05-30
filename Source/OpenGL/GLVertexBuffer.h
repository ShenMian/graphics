// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VertexBuffer.h"
#include "GLBuffer.h"
#include "GLVertexArray.h"
#include <glad/glad.h>

class GLVertexBuffer : public VertexBuffer
{
public:
	GLVertexBuffer(const void* data, size_t size, const VertexAttributes& fmt, Buffer::Usage usage);

	Buffer& getBuffer() override;

	void bind() override;

private:
	GLBuffer      buffer;
	GLVertexArray vao;
};
