// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include <cstddef>
#include <glad/glad.h>

class GLBuffer : public Buffer
{
public:
	GLBuffer(size_t size, Type type, Usage usage);
	virtual ~GLBuffer();

	void map(size_t size = -1, size_t offset = 0) override;
	void unmap() override;
	void flush(size_t size = -1, size_t offset = 0) override;

	void bind();
	operator GLuint() noexcept;
	operator GLuint() const noexcept;

private:
	GLuint handle;
	GLenum glType;
};
