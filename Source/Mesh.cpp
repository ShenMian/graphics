// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Mesh.h"

Mesh::Mesh(const std::string& name, std::shared_ptr<VertexBuffer> vertexBuffer, std::shared_ptr<IndexBuffer> indexBuffer)
	: name(name), vertexBuffer(vertexBuffer), indexBuffer(indexBuffer)
{
}

const std::string& Mesh::getName() const
{
	return name;
}

std::shared_ptr<VertexBuffer> Mesh::getVertexBuffer() const
{
	return vertexBuffer;
}

std::shared_ptr<IndexBuffer> Mesh::getIndexBuffer() const
{
	return indexBuffer;
}
