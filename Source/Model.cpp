// Copyright 2021 SMS
// License(Apache-2.0)

#include "Model.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include <assimp/postprocess.h>
#include <cassert>
#include <future>
#include <vector>
#include <algorithm>
#include <concepts>
#include <cstring>

namespace fs = std::filesystem;

void Model::load(const fs::path& path, std::error_code& ec)
{
	if(!fs::exists(path))
	{
		ec = std::make_error_code(std::errc::no_such_file_or_directory);
		return;
	}

	// 从文件导入场景数据
	scene = importer.ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
	{
		ec = std::make_error_code(Errc::bed_file);
		return;
	}

	loadNode(scene->mRootNode);

	ec = std::error_code();
}

void Model::loadAsync(const fs::path& path, std::function<void(std::error_code)> callback)
{
	std::thread([=]()
	{
		std::error_code ec;
		load(path, ec);
		callback(ec);
	}).detach();

#if 0
	static auto future = std::async([=]()
	{
		std::error_code ec;
		load(path, ec);
		callback(ec);
	});
#endif
}

void Model::loadNode(aiNode* node)
{
	// 加载网格
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		auto id = node->mMeshes[i];
		loadMesh(scene->mMeshes[id]);
	}

	// 加载材质
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		auto children = node->mChildren[i];
		loadNode(children);
	}
}

static_assert(std::same_as<ai_real, float>);

void Model::loadMesh(aiMesh* mesh)
{
	std::vector<VertexBuffer::value_type> vertices;
	std::vector<IndexBuffer::value_type>  indices;

	// 获取顶点数据
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		auto v = mesh->mVertices[i];

		VertexBuffer::value_type vertex;

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
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// 获取材质数据
}

/*
void Model::save(const fs::path& path, std::error_code& ec)
{
	assert(scene != nullptr);

	// 导出场景数据到文件
	auto ret = exporter.Export(scene, path.extension().string(), path.string());
	if(ret == aiReturn_FAILURE)
		ec = std::make_error_code(std::errc::no_message);

	ec = std::error_code();
}

void Model::saveAsync(const fs::path& path, std::function<void(std::error_code)> callback)
{
	std::thread([=]()
	{
		std::error_code ec;
		save(path, ec);
		callback(ec);
	}).detach();

#if 0
	static auto future = std::async([=]()
	{
		std::error_code ec;
		save(path, ec);
		callback(ec);
	});
#endif
}
*/

// static std::vector<std::future<void>> futures;
// futures.erase(std::remove_if(futures.begin(), futures.end(), [](std::future<void>& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }), futures.end());
