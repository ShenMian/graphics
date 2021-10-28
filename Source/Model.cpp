﻿// Copyright 2021 SMS
// License(Apache-2.0)

#include "Model.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <assimp/postprocess.h>
#include <meshoptimizer.h>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstring>
#include <future>
#include <vector>

namespace fs = std::filesystem;

namespace
{

void optimize(std::vector<unsigned int>& indices, std::vector<Vertex>& vertices)
{
	meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
	meshopt_optimizeOverdraw(indices.data(), indices.data(), indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Vertex), 1.05f);
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(), sizeof(Vertex));
}

}

void Model::load(const fs::path& path)
{
	if(!fs::exists(path) && !fs::is_regular_file(path))
		throw std::exception("no such file or directory");

	// 从文件导入场景数据
	scene = importer.ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		throw std::exception("scene data incomplete");

	loadNode(scene->mRootNode);
}

void Model::loadAsync(const fs::path& path, std::function<void(std::error_code)> callback) noexcept
{
	std::thread([=]()
	{
		std::error_code ec;
		load(path);
		if(callback)
			callback(ec);
	}).detach();
}

void Model::loadNode(aiNode* node)
{
	// 加载网格
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]]);

	// 加载材质
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i]);
}

void Model::loadMesh(aiMesh* mesh)
{
	std::vector<Vertex>       vertices;
	std::vector<unsigned int> indices;

	// 获取顶点数据
	static_assert(std::same_as<ai_real, float>);
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		auto v = mesh->mVertices[i];

		Vertex vertex;

		// 获取坐标
		std::memcpy(&vertex.normal, &mesh->mVertices[i], sizeof(vertex.position));

		// 获取法向量
		if(mesh->HasNormals())
			std::memcpy(&vertex.normal, &mesh->mNormals[i], sizeof(vertex.normal));

		if(mesh->mTextureCoords[0])
		{
			// 获取 UV 坐标(纹理坐标)
			vertex.uv.x = mesh->mTextureCoords[0][i].x;
			vertex.uv.y = mesh->mTextureCoords[0][i].y;

			// 获取 tangent
			std::memcpy(&vertex.tangent, &mesh->mTangents[i], sizeof(vertex.tangent));

			// 获取 bitangent
			std::memcpy(&vertex.bitangent, &mesh->mBitangents[i], sizeof(vertex.bitangent));
		}

		vertices.push_back(vertex);
	}

	// 获取索引数据
	// static_assert(typeid(aiFace::mIndices) == typeid(IndexBuffer::value_type*));
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	optimize(indices, vertices);

	auto indexBuffer  = IndexBuffer::create(indices);
	auto vertexBuffer = VertexBuffer::create(vertices);

	// 获取材质数据
	// TODO
}

/*
void Model::save(const fs::path& path)
{
	assert(scene != nullptr);

	// 导出场景数据到文件
	auto ret = exporter.Export(scene, path.extension().string(), path.string());
	if(ret == aiReturn_FAILURE)
		throw std::exception("export scene data failed");
}

void Model::saveAsync(const fs::path& path, std::function<void(std::error_code)> callback) noexcept
{
	std::thread([=]()
	{
		std::error_code ec;
		save(path, ec);
		if(callback)
			callback(ec);
	}).detach();
}
*/

// static std::vector<std::future<void>> futures;
// futures.erase(std::remove_if(futures.begin(), futures.end(), [](std::future<void>& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }), futures.end());