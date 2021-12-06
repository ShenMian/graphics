// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Mesh.h"

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

void Mesh::setName(const std::string& name)
{
	this->name = name;
}

void Mesh::setVertexBuffer(std::shared_ptr<VertexBuffer> buffer)
{
	vertexBuffer = buffer;
}

void Mesh::setIndexBuffer(std::shared_ptr<IndexBuffer> buffer)
{
	indexBuffer = buffer;
}

void Mesh::setMaterial(const Material& mat)
{
	material = mat;
}

const Material& Mesh::getMaterial() const
{
	return material;
}
