// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Mesh.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <meshoptimizer.h>

Mesh::Mesh(std::string_view name, const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices,
           Material* mat)
    : name(name), vertices(vertices), indices(indices), material(mat), vertexCount(vertices.size()),
      indexCount(indices.size())
{
	optimize();
	createBuffers();
}

float Mesh::compress()
{
	assert(!isCompressed());
	assert(vertexBuffer.use_count() == 1 && indexBuffer.use_count() == 1);

	vertexBuffer.reset();
	indexBuffer.reset();

	vbuf.resize(meshopt_encodeVertexBufferBound(vertexCount, sizeof(Vertex)));
	vbuf.resize(meshopt_encodeVertexBuffer(vbuf.data(), vbuf.size(), vertices.data(), vertexCount, sizeof(Vertex)));
	vbuf.shrink_to_fit();

	ibuf.resize(meshopt_encodeIndexBufferBound(indexCount, vertexCount));
	ibuf.resize(meshopt_encodeIndexBuffer(ibuf.data(), ibuf.size(), indices.data(), indexCount));
	ibuf.shrink_to_fit();

	const float ratio = static_cast<float>(vbuf.size() + ibuf.size()) /
	                    (vertices.size() * sizeof(Vertex) + indices.size() * sizeof(unsigned int));

	vertices.clear();
	indices.clear();

	return ratio;
}

void Mesh::decompress()
{
	assert(isCompressed());

	const auto resvb =
	    meshopt_decodeVertexBuffer(vertices.data(), vertexCount, sizeof(Vertex), vbuf.data(), vbuf.size());
	const auto resib = meshopt_decodeIndexBuffer(indices.data(), indexCount, ibuf.data(), ibuf.size());
	assert(resvb == 0 && resib == 0);

	createBuffers();

	vbuf.clear();
	ibuf.clear();
}

bool Mesh::isCompressed() const noexcept
{
	return !vbuf.empty() || !ibuf.empty();
}

void Mesh::optimize()
{
	meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
	meshopt_optimizeOverdraw(indices.data(), indices.data(), indices.size(), &vertices[0].position.x(), vertices.size(),
	                         sizeof(Vertex), 1.05f);
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(),
	                            sizeof(Vertex));
}

void Mesh::createBuffers()
{
	VertexFormat format = {
	    {"position", Format::RGB32F}, {"normal", Format::RGB32F},    {"uv", Format::RG32F},
	    {"tangent", Format::RGB32F},  {"bitangent", Format::RGB32F},
	};
	vertexBuffer = VertexBuffer::create(this->vertices, format);
	indexBuffer  = IndexBuffer::create(this->indices);
}
