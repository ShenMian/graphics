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

	struct Info
	{
		uint32_t triangles = 0;
		uint32_t vertices = 0;
		uint32_t indices = 0;
	};

    Mesh() = default;

	Mesh(const std::string& name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const Material& mat);

	/**
	 * @brief 获取名称.
	 */
	[[nodiscard]] const std::string& getName() const;

	/**
	 * @brief 获取顶点缓冲区.
	 */
    [[nodiscard]] std::shared_ptr<VertexBuffer> getVertexBuffer() const;

	/**
	 * @brief 获取索引缓冲区.
	 */
    [[nodiscard]] std::shared_ptr<IndexBuffer> getIndexBuffer() const;

	/**
	 * @brief 获取材质.
	 */
    [[nodiscard]] const Material& getMaterial() const;

	/**
	 * @brief 获取信息.
	 */
	[[nodiscard]] const Info& getInfo() const;

	[[nodiscard]] bool isCompressed() const;

	/**
	 * @brief 压缩.
	 */
	void compress();

	/**
	 * @brief 解压.
	 */
	void decompress();


	void setName(const std::string& name);
	void setVertexBuffer(std::shared_ptr<VertexBuffer> buffer);
	void setIndexBuffer(std::shared_ptr<IndexBuffer> buffer);
	void setMaterial(const Material& mat);

private:
	void optimize();

	std::string                   name;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer>  indexBuffer;
	Material                      material;

	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	Info                      info;

	std::vector<uint8_t> compressedVertices;
	std::vector<uint8_t> compressedIndices;
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
