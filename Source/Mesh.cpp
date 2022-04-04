// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Mesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <meshoptimizer.h>
#include <cmath>

namespace
{

// 压缩索引缓冲区
void compressIndices(std::vector<uint8_t>& buffer, const void* indices, size_t indexCount, size_t vertexCount)
{
	buffer.resize(meshopt_encodeIndexBufferBound(indexCount, vertexCount));
	buffer.resize(meshopt_encodeIndexBuffer(buffer.data(), buffer.size(), reinterpret_cast<const unsigned int*>(indices), indexCount));
}

// 压缩顶点缓冲区
void compressVertices(std::vector<uint8_t>& buffer, const void* vertices, size_t vertexCount, size_t vertexSize)
{
	buffer.resize(meshopt_encodeVertexBufferBound(vertexCount, vertexSize));
	buffer.resize(meshopt_encodeVertexBuffer(buffer.data(), buffer.size(), vertices, vertexCount, vertexSize));
}

// 解压索引缓冲区
void decompressIndices(void* indices, size_t indexCount, const std::vector<uint8_t>& buffer)
{
	meshopt_decodeIndexBuffer(reinterpret_cast<unsigned int*>(indices), indexCount, buffer.data(), buffer.size());
}

// 压缩顶点缓冲区
void decompressVertices(void* vertices, size_t vertexCount, size_t vertexSize, const std::vector<uint8_t>& buffer)
{
	meshopt_decodeVertexBuffer(vertices, vertexCount, vertexSize, buffer.data(), buffer.size());
}

}

Mesh::Mesh(std::string_view name, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const Material& mat)
	: name(name), vertices(std::move(vertices)), indices(std::move(indices)), material(mat)
{
	optimize();

	info.triangles = this->indices.size() / 3;
	info.indices = this->indices.size();
	info.vertices = this->vertices.size();

	// TODO: 与 decompress 中的变量重复
	VertexAttributes format = {
		{"position", Format::RGB32F},
		{"normal", Format::RGB32F},
		{"uv", Format::RG32F},
		{"tangent", Format::RGB32F},
		{"bitangent", Format::RGB32F},
	};
	vertexBuffer = VertexBuffer::create(this->vertices, format);
	indexBuffer = IndexBuffer::create(this->indices);
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

const Material& Mesh::getMaterial() const
{
	return material;
}

const Mesh::Info& Mesh::getInfo() const
{
	return info;
}

bool Mesh::isCompressed() const
{
	return !compressedIndices.empty() || !compressedVertices.empty();
}

void Mesh::compress()
{
	// FIXME: 存在内存泄露, 此处 indexBuffer 和 vertexBuffer 是引用计数很大, 而不是预期的 1.
	// 随时间的增加而增加

	compressIndices(compressedIndices, indices.data(), indices.size(), vertices.size());
	indices.clear();
	compressedIndices.shrink_to_fit();
	indexBuffer = nullptr;

	compressVertices(compressedVertices, vertices.data(), vertices.size(), sizeof(Vertex));
	vertices.clear();
	compressedVertices.shrink_to_fit();
	vertexBuffer = nullptr;
}

void Mesh::decompress()
{
	indices.resize(info.indices);
	decompressIndices(indices.data(), info.indices, compressedIndices);
	compressedIndices.clear();

	vertices.resize(info.vertices);
	decompressVertices(vertices.data(), info.vertices, sizeof(Vertex), compressedVertices);
	compressedVertices.clear();

	VertexAttributes format = {
		{"position", Format::RGB32F},
		{"normal", Format::RGB32F},
		{"uv", Format::RG32F},
		{"tangent", Format::RGB32F},
		{"bitangent", Format::RGB32F},
	};
	vertexBuffer = VertexBuffer::create(this->vertices, format);
	indexBuffer = IndexBuffer::create(this->indices);
}

void Mesh::optimize()
{
	meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
	meshopt_optimizeOverdraw(indices.data(), indices.data(), indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Mesh::Vertex), 1.05f);
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(), sizeof(Mesh::Vertex));
}

Mesh::Info& Mesh::Info::operator+=(const Mesh::Info& rhs)
{
	triangles += rhs.triangles;
	vertices += rhs.vertices;
	indices += rhs.indices;
	return *this;
}
