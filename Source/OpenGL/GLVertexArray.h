// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <glad/glad.h>

class VertexFormat;

class GLVertexArray
{
public:
	GLVertexArray();
	virtual ~GLVertexArray();

	void bind();

	void build(const VertexFormat& fmt);

private:
	GLuint handle;
};
