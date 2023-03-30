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
	create_buffers();
}

float Mesh::compress()
{
	assert(!is_compressed());
	assert(vertexBuffer.use_count() == 1 && indexBuffer.use_count() == 1);

	vertexBuffer.reset();
	indexBuffer.reset();

	vbuf_.resize(meshopt_encodeVertexBufferBound(vertexCount, sizeof(Vertex)));
	vbuf_.resize(meshopt_encodeVertexBuffer(vbuf_.data(), vbuf_.size(), vertices.data(), vertexCount, sizeof(Vertex)));
	vbuf_.shrink_to_fit();

	ibuf_.resize(meshopt_encodeIndexBufferBound(indexCount, vertexCount));
	ibuf_.resize(meshopt_encodeIndexBuffer(ibuf_.data(), ibuf_.size(), indices.data(), indexCount));
	ibuf_.shrink_to_fit();

	const float ratio = static_cast<float>(vbuf_.size() + ibuf_.size()) /
	                    (vertices.size() * sizeof(Vertex) + indices.size() * sizeof(unsigned int));

	vertices.clear();
	indices.clear();

	return ratio;
}

void Mesh::decompress()
{
	assert(is_compressed());

	const auto resvb =
	    meshopt_decodeVertexBuffer(vertices.data(), vertexCount, sizeof(Vertex), vbuf_.data(), vbuf_.size());
	const auto resib = meshopt_decodeIndexBuffer(indices.data(), indexCount, ibuf_.data(), ibuf_.size());
	assert(resvb == 0 && resib == 0);

	create_buffers();

	vbuf_.clear();
	ibuf_.clear();
}

bool Mesh::is_compressed() const noexcept
{
	return !vbuf_.empty() || !ibuf_.empty();
}

void Mesh::optimize()
{
	meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
	meshopt_optimizeOverdraw(indices.data(), indices.data(), indices.size(), &vertices[0].position.x(), vertices.size(),
	                         sizeof(Vertex), 1.05f);
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(),
	                            sizeof(Vertex));
}

void Mesh::create_buffers()
{
	VertexFormat format = {
	    {"position", Format::RGB32F}, {"normal", Format::RGB32F},    {"uv", Format::RG32F},
	    {"tangent", Format::RGB32F},  {"bitangent", Format::RGB32F},
	};
	vertexBuffer = VertexBuffer::create(this->vertices, format);
	indexBuffer  = IndexBuffer::create(this->indices);
}
