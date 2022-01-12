﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "IndexBuffer.h"
#include <glad/glad.h>

class GLIndexBuffer : public IndexBuffer
{
public:
	GLIndexBuffer(const unsigned int* data, size_t size, Usage usage);
	virtual ~GLIndexBuffer();

	void map() override;
	void unmap() override;

	void write(const void* data, size_t size) override;
	void bind() override;

private:
	GLuint handle;
	void*  data = nullptr;
};
