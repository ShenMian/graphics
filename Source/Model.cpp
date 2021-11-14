// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Model.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "Core/Image.h"
#include <assimp/postprocess.h>
#include <meshoptimizer.h>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstring>
#include <future>
#include <vector>

#include "Core/Timer.hpp"

namespace fs = std::filesystem;

namespace
{

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Vector3 tangent;
	Vector3 bitangent;

	auto operator<=>(const Vertex&) const = default;
};

// TODO: 属于 Mesh 的职责.
// 优化网格
void optimize(std::vector<unsigned int>& indices, std::vector<Vertex>& vertices)
{
	meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
	meshopt_optimizeOverdraw(indices.data(), indices.data(), indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Vertex), 1.05f);
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(), sizeof(Vertex));
}

// 获取顶点数据
void loadVertices(std::vector<Vertex>& vertices, aiMesh* mesh)
{
	static_assert(std::same_as<ai_real, float>);
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		auto v = mesh->mVertices[i];

		Vertex vertex;

		// 获取坐标
		std::memcpy(&vertex.position, &mesh->mVertices[i], sizeof(vertex.position));

		// TODO: 性能测试.
		// vertex.position.x = mesh->mVertices[i][0];
		// vertex.position.x = mesh->mVertices[i][1];
		// vertex.position.x = mesh->mVertices[i][2];

		// 获取法向量
		if(mesh->HasNormals())
			std::memcpy(&vertex.normal, &mesh->mNormals[i], sizeof(vertex.normal));

		// 获取 UV 坐标(纹理坐标)
		if(mesh->mTextureCoords[0])
			std::memcpy(&vertex.uv, &mesh->mTextureCoords[0][i], sizeof(vertex.uv));

		std::memcpy(&vertex.tangent, &mesh->mTangents[i], sizeof(vertex.tangent));       // 获取 tangent
		std::memcpy(&vertex.bitangent, &mesh->mBitangents[i], sizeof(vertex.bitangent)); // 获取 bitangent

		vertices.push_back(vertex);
	}
}

// 获取索引数据
void loadIndices(std::vector<unsigned int>& indices, aiMesh* mesh)
{
	// static_assert(std::same_as<decltype(aiFace::mIndices), unsigned int*>);
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

}

void Model::load(const fs::path& path)
{
	if(!fs::exists(path) && !fs::is_regular_file(path))
		throw std::exception("no such file or directory");

	this->path = path;

	Timer timer; // TODO: debug

	// 从文件导入场景数据
	scene = importer.ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		throw std::exception(importer.GetErrorString());

	name = scene->mName.C_Str();

	loadNode(scene->mRootNode);
	puts(std::format("网格载入完毕: {}s", timer.getSeconds()).c_str()); // TODO: debug
}

void Model::loadAsync(const fs::path& path, std::function<void(std::error_code)> callback) noexcept
{
	try
	{
		std::thread([=]()
		{
			std::error_code ec;
			load(path);
			if(callback)
				callback(ec);
		}).detach();
	}
	catch(...)
	{
		assert(false);
	}
}

const std::vector<Mesh> Model::getMeshs() const
{
	return meshs;
}

void Model::loadNode(aiNode* node)
{
	// 加载网格
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
		loadMesh(scene->mMeshes[node->mMeshes[i]]);

	// 加载其余节点
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i]);
}

void Model::loadMesh(aiMesh* mesh)
{
	static auto currScene = scene; // TODO: debug
	static unsigned int i = 0;
	if(currScene != scene)
		currScene = scene, i = 0;
	printf(std::format("载入网格: {:>3}/{:<3}\r", ++i, scene->mNumMeshes).c_str());

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	loadVertices(vertices, mesh);
	loadIndices(indices, mesh);

	optimize(indices, vertices);

	VertexFormat format = {
		{"position", Format::RGB32F},
		{"normal", Format::RGB32F},
		{"uv", Format::RG32F},
		{"tangent", Format::RGB32F},
		{"bitangent", Format::RGB32F},
	};

	auto vertexBuffer = VertexBuffer::create(vertices, format);
	auto indexBuffer  = IndexBuffer::create(indices);

	std::string name = mesh->mName.C_Str();

	// TODO: 创建包围盒
	// for(auto& vertex : vertices)
	// mesh->mAABB;

	// vertices 应该由 Model 管理, 传递给 Mesh 构造函数. Mesh 在已有的基础上继续添加顶点数据.

	meshs.emplace_back(name, vertexBuffer, indexBuffer);

	// loadMaterial(&meshs.back(), mesh);
}

void Model::loadMaterial(Mesh* mesh, aiMesh* aiMesh)
{
	const auto dir = path.parent_path();
	const auto aiMat = scene->mMaterials[aiMesh->mMaterialIndex];

	auto loadTexture = [&](aiTextureType type)->std::shared_ptr<Texture>
	{
		for(unsigned int i = 0; i < aiMat->GetTextureCount(type); i++)
		{
			aiString aiPath;
			aiMat->GetTexture(type, 0, &aiPath);
			const auto path = dir / aiPath.C_Str();
			// if(fs::exists(path))
			return Texture::create(Image(path));
		}
		return nullptr;
	};

	mesh->material.name = aiMat->GetName().C_Str();

	mesh->material.pbr.albedo = loadTexture(aiTextureType_BASE_COLOR);
	mesh->material.pbr.normals = loadTexture(aiTextureType_NORMAL_CAMERA);
	mesh->material.pbr.emissive = loadTexture(aiTextureType_EMISSION_COLOR);
	mesh->material.pbr.metallic = loadTexture(aiTextureType_METALNESS);
	mesh->material.pbr.roughness = loadTexture(aiTextureType_DIFFUSE_ROUGHNESS);
	mesh->material.pbr.ao = loadTexture(aiTextureType_AMBIENT_OCCLUSION);
	
	mesh->material.diffuse = loadTexture(aiTextureType_DIFFUSE);
	mesh->material.specular = loadTexture(aiTextureType_SPECULAR);
	mesh->material.ambient = loadTexture(aiTextureType_AMBIENT);
	mesh->material.emissive = loadTexture(aiTextureType_EMISSIVE);
	mesh->material.height = loadTexture(aiTextureType_HEIGHT);
	mesh->material.normals = loadTexture(aiTextureType_NORMALS);
	mesh->material.shininess = loadTexture(aiTextureType_SHININESS);
	mesh->material.opacity = loadTexture(aiTextureType_OPACITY);
}

/*
void Model::save(const fs::path& path)
{
	assert(scene != nullptr);

	// 导出场景数据到文件
	auto ret = exporter.Export(scene, path.extension().string(), path.string());
	if(ret == aiReturn_FAILURE)
		throw std::exception(exporter.GetErrorString());
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
