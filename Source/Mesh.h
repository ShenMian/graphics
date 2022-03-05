// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Math/Math.hpp"
#include "Material.h"
#include <memory>
#include <string>
#include <vector>

class IndexBuffer;
class VertexBuffer;

/** @addtogroup model
 *  @{
 */

 /**
  * @brief 网格.
  */
class Mesh
{
public:
    struct Vertex;

    Mesh() = default;

	Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const Material& mat);

	void setName(const std::string& name);

	[[nodiscard]] const std::string& getName() const;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> buffer);

    [[nodiscard]] std::shared_ptr<VertexBuffer> getVertexBuffer() const;

	void setIndexBuffer(std::shared_ptr<IndexBuffer> buffer);

    [[nodiscard]] std::shared_ptr<IndexBuffer> getIndexBuffer() const;

	void setMaterial(const Material& mat);

    [[nodiscard]] const Material& getMaterial() const;

	[[nodiscard]] uint32_t getTriangleCount() const;

	[[nodiscard]] uint32_t getVertexCount() const;

	bool isCompressed() const;
	void compress();
	void decompress();

private:
	void optimize();

	std::string                   name;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer>  indexBuffer;
	Material                      material;

	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;

	std::vector<uint8_t> compressedVertices;
	std::vector<uint8_t> compressedIndices;
	uint32_t vertexCount;
	uint32_t indexCount;
};

struct Mesh::Vertex
{
    Vector3 position;
    Vector3 normal;
    Vector2 uv;
    Vector3 tangent;
    Vector3 bitangent;

    auto operator<=>(const Vertex&) const = default;
};

/** @}*/
