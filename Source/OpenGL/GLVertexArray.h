// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <glad/glad.h>

class VertexLayout;

class GLVertexArray
{
public:
	GLVertexArray();
	virtual ~GLVertexArray();

	void bind();

	void build(const VertexLayout& fmt);

	// void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
	// void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

private:
	GLuint handle;
	GLuint attribIndex = 0;
};
