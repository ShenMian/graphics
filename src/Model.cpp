// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Model.h"
#include <cassert>
#include <fstream>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace fs = std::filesystem;

struct Header
{
	uint8_t  magic[4] = {'C', 'L', 'E', 'M'};
	uint32_t version  = 0;
};

void Model::load(const fs::path& path)
{
	if(!fs::exists(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));
	std::ifstream file(path, std::ios::binary);
	if(!file)
		throw std::runtime_error(fmt::format("failed to open file: {}", path));

	Header header;
	file.read(reinterpret_cast<char*>(&header), sizeof(header));
	assert(header.version == 0);

	// TODO: name, materials, bones, animations
	size_t size;
	file >> size;
	for(size_t i = 0; i < size; i++)
	{
		file >> size;
		std::vector<unsigned int> indices(size);
		file.read(reinterpret_cast<char*>(indices.data()), indices.size() * sizeof(unsigned int));

		file >> size;
		std::vector<Mesh::Vertex> vertices(size);
		file.read(reinterpret_cast<char*>(vertices.data()), vertices.size() * sizeof(Mesh::Vertex));

		meshes.emplace_back("", vertices, indices);
	}
}

void Model::save(const fs::path& path)
{
	std::ofstream file(path, std::ios::binary);
	if(!file)
		throw std::runtime_error(fmt::format("failed to open file: {}", path));

	Header header;
	file.write(reinterpret_cast<const char*>(&header), sizeof(header));

	// TODO: name, materials, bones, animations
	file << meshes.size();
	for(const auto& mesh : meshes)
	{
		file << mesh.indices.size();
		file.write(reinterpret_cast<const char*>(mesh.indices.data()), mesh.vertices.size() * sizeof(unsigned int));
		file << mesh.vertices.size();
		file.write(reinterpret_cast<const char*>(mesh.vertices.data()), mesh.vertices.size() * sizeof(Mesh::Vertex));
	}
}
