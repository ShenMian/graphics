// Copyright 2021 SMS
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
	Mesh(const std::string& name, std::shared_ptr<VertexBuffer> vertexBuffer,
		std::shared_ptr<IndexBuffer> indexBuffer);

	const std::string& getName() const;

	Material material;

private:
	std::string                   name;
	std::shared_ptr<IndexBuffer>  indexBuffer;
	std::shared_ptr<VertexBuffer> vertexBuffer;
};