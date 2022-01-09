﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VertexBuffer.h"
#include "GLVertexArray.h"
#include <glad/glad.h>

class GLVertexBuffer : public VertexBuffer
{
public:
	GLVertexBuffer(const void* data, size_t size, uint32_t count, const VertexLayout& fmt, Usage usage);
	virtual ~GLVertexBuffer();

	void bind() override;

private:
	GLuint        handle;
	GLVertexArray vao;
};
