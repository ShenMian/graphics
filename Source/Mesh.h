// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Material.h"
#include <memory>
#include <string>

class IndexBuffer;
class VertexBuffer;

class Mesh
{
public:
	Mesh() = default;

	void setName(const std::string& name);

	const std::string& getName() const;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> buffer);

	std::shared_ptr<VertexBuffer> getVertexBuffer() const;

	void setIndexBuffer(std::shared_ptr<IndexBuffer> buffer);

	std::shared_ptr<IndexBuffer> getIndexBuffer() const;

	void setMaterial(const Material& mat);

	const Material& getMaterial() const;

private:
	std::string                   name;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer>  indexBuffer;
	Material                      material;
};
