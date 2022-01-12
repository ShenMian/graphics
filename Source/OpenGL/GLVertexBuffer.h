// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VertexBuffer.h"
#include "GLVertexArray.h"
#include <glad/glad.h>

class GLVertexBuffer : public VertexBuffer
{
public:
	GLVertexBuffer(const void* data, size_t size, const VertexLayout& fmt, Usage usage);
	virtual ~GLVertexBuffer();

	void map() override;
	void unmap() override;

	void write(const void* data, size_t size) override;
	void bind() override;

private:
	GLuint        handle;
	void*         data = nullptr;
	GLVertexArray vao;
};
