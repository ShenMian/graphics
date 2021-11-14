// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

class VertexFormat;

class GLVertexArray
{
public:
	using handle_type = unsigned int;

	GLVertexArray();
	virtual ~GLVertexArray();

	void bind();

	void build(const VertexFormat& fmt);

	// void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
	// void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);

private:
	handle_type handle;
	handle_type attribIndex = 0;
};