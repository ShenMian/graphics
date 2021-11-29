// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VertexBuffer.h"
#include "GLVertexArray.h"
#include <glad/glad.h>

class GLVertexBuffer : public VertexBuffer
{
public:
	using handle_type = unsigned int;

	GLVertexBuffer(const void* data, size_t size, size_t count, const VertexFormat& fmt, Usage usage);
	virtual ~GLVertexBuffer();

	void bind() override;

private:
	handle_type   handle;
	GLVertexArray vao;
};
