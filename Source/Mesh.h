// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <math/math.hpp>
#include <memory>
#include <string>
#include <vector>

class IndexBuffer;
class VertexBuffer;
struct Material;

/** @addtogroup model
 *  @{
 */

/**
 * @brief 网格.
 */
class Mesh
{
public:
	struct Vertex
	{
		Vector3 position;
		Vector3 normal;
		Vector2 uv;
		Vector3 tangent;
		Vector3 bitangent;
	};

	Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
	     Material* mat = nullptr);

	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr<IndexBuffer>  indexBuffer;

	std::string               name;
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;
	Material*                 material = nullptr;

	uint32_t vertexCount = 0;
	uint32_t indexCount  = 0;

	/**
	 * @brief 压缩.
	 *
	 * @return 压缩率.
	 *
	 * @warning 压缩前应先进行优化, 以提高压缩率.
	 */
	float compress();

	/**
	 * @brief 解压.
	 */
	void decompress();

	/**
	 * 是否已经过压缩.
	 */
	bool isCompressed() const noexcept;

private:
	void optimize();
	void createBuffers();

	std::vector<unsigned char> vbuf;
	std::vector<unsigned char> ibuf;
};

/** @}*/
