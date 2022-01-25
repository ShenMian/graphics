// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include <glad/glad.h>
#include <cstddef>

class GLBuffer : public Buffer
{
public:
	GLBuffer(size_t size, Type type, Usage usage);
	virtual ~GLBuffer();

	void map(size_t size = -1, size_t offset = 0) override;
	void unmap() override;
	void flush(size_t size = -1, size_t offset = 0) override;

	void write(const void* data, size_t size, size_t offset = 0);

	void bind();
	operator GLuint() const;

private:
	GLuint handle;
	GLenum glType;
};
