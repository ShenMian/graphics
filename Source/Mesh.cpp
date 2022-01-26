// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Mesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <meshoptimizer.h>

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

void Mesh::setVertexBuffer(std::shared_ptr<VertexBuffer> buf)
{
	vertexBuffer = buf;
}

void Mesh::setIndexBuffer(std::shared_ptr<IndexBuffer> buf)
{
	indexBuffer = buf;
}

void Mesh::setMaterial(const Material& mat)
{
	material = mat;
}

const Material& Mesh::getMaterial() const
{
	return material;
}

void Mesh::compress()
{
	// TODO
	/*
	{
		std::vector<uint8_t> tmp;
		auto& buf = indexBuffer->getData();
		compressIndices(tmp, buf.data(), indexBuffer->getCount(), vertexBuffer->getCount());
		buf = tmp;
		buf.shrink_to_fit();
		indexBuffer->flash();
	}
	{
		std::vector<uint8_t> tmp;
		auto& buf = vertexBuffer->getData();
		compressVertices(tmp, buf.data(), vertexBuffer->getCount(), vertexBuffer->getFormat().getStride());
		buf = tmp;
		buf.shrink_to_fit();
		vertexBuffer->flash();
	}
	*/
}

void Mesh::decompress()
{
	// TODO
	/*
	{
		std::vector<uint8_t> tmp;
		auto& buf = indexBuffer->getData();
		tmp.resize(indexBuffer->getSize());
		decompressIndices(tmp.data(), indexBuffer->getCount(), buf);
		buf = tmp;
		indexBuffer->flash();
	}
	{
		std::vector<uint8_t> tmp;
		auto& buf = vertexBuffer->getData();
		tmp.resize(vertexBuffer->getSize());
		decompressVertices(tmp.data(), vertexBuffer->getCount(), vertexBuffer->getFormat().getStride(), buf);
		buf = tmp;
		vertexBuffer->flash();
	}
	*/
}
