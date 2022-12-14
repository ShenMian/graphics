// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <memory>
#include <glad/glad.h>

class GLBuffer;
class VertexFormat;

class GLVertexArray
{
public:
	GLVertexArray();
	virtual ~GLVertexArray();

	void bind();

	void build(const VertexFormat& fmt, GLBuffer& vbo);

private:
	GLuint handle;
};
