// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <glad/glad.h>
#include <cstddef>

class GLBuffer
{
public:
	GLBuffer(size_t size, GLenum type);
	virtual ~GLBuffer();

	void map(size_t size = -1, size_t offset = 0);
	void unmap();

	void write(const void* data, size_t size, size_t offset = 0);

	void* getData();

	void bind();

	operator GLuint() const;

private:
	void allocate(size_t size);

	GLuint handle;
	size_t size;
	GLenum type;
	void*  data = nullptr;
};
