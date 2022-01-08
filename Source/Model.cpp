// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Model.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Material.h"
#include "Core/Image.h"
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstring>
#include <future>
#include <meshoptimizer.h>
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

// 获取顶点数据
template <typename Vertex>
void loadVertices(std::vector<Vertex>& vertices, const aiMesh* mesh)
{
	static_assert(std::same_as<ai_real, float>);

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// 获取坐标
		std::memcpy(&vertex.position, &mesh->mVertices[i], sizeof(vertex.position));

		// 获取法向量
		if(mesh->HasNormals())
			std::memcpy(&vertex.normal, &mesh->mNormals[i], sizeof(vertex.normal));

		// 检查是否存在纹理坐标
		if(mesh->mTextureCoords[0])
		{
			std::memcpy(&vertex.uv, &mesh->mTextureCoords[0][i], sizeof(vertex.uv));         // 获取纹理坐标
			std::memcpy(&vertex.tangent, &mesh->mTangents[i], sizeof(vertex.tangent));       // 获取 tangent
			std::memcpy(&vertex.bitangent, &mesh->mBitangents[i], sizeof(vertex.bitangent)); // 获取 bitangent
		}

		vertices.push_back(vertex);
	}
}

// 获取索引数据
void loadIndices(std::vector<unsigned int>& indices, const aiMesh* mesh)
{
	static_assert(std::same_as<decltype(aiMesh::mNumFaces), unsigned int>);
	static_assert(std::same_as<decltype(aiFace::mIndices), unsigned int*>);

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		const auto& face = mesh->mFaces[i];

		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
}

// 优化网格
void optimize(std::vector<unsigned int>& indices, std::vector<Vertex>& vertices)
{
	meshopt_optimizeVertexCache(indices.data(), indices.data(), indices.size(), vertices.size());
	meshopt_optimizeOverdraw(indices.data(), indices.data(), indices.size(), &vertices[0].position.x, vertices.size(), sizeof(Vertex), 1.05f);
	meshopt_optimizeVertexFetch(vertices.data(), indices.data(), indices.size(), vertices.data(), vertices.size(), sizeof(Vertex));
}

void loadMaterial(Material& mat, const aiMesh* aMesh, const aiScene* scene, const fs::path path)
{
	const auto dir = path.parent_path();
	const auto aMat = scene->mMaterials[aMesh->mMaterialIndex];
	auto loadTexture = [&](aiTextureType type)->std::shared_ptr<Texture>
	{
		for(unsigned int i = 0; i < aMat->GetTextureCount(type); i++)
		{
			aiString aPath;
			aMat->GetTexture(type, 0, &aPath);
			const auto path = dir / aPath.C_Str();
			// if(!fs::exists(path))
			// 	return nullptr;
			return Texture::create(path);
		}
		return nullptr;
	};

	mat.name = aMat->GetName().C_Str();

	mat.pbr.albedo = loadTexture(aiTextureType_BASE_COLOR);
	mat.pbr.normals = loadTexture(aiTextureType_NORMAL_CAMERA);
	mat.pbr.emissive = loadTexture(aiTextureType_EMISSION_COLOR);
	mat.pbr.metallic = loadTexture(aiTextureType_METALNESS);
	mat.pbr.roughness = loadTexture(aiTextureType_DIFFUSE_ROUGHNESS);
	mat.pbr.ao = loadTexture(aiTextureType_AMBIENT_OCCLUSION);

	mat.diffuse = loadTexture(aiTextureType_DIFFUSE);
	mat.specular = loadTexture(aiTextureType_SPECULAR);
	mat.ambient = loadTexture(aiTextureType_AMBIENT);
	mat.emissive = loadTexture(aiTextureType_EMISSIVE);
	mat.height = loadTexture(aiTextureType_HEIGHT);
	mat.normals = loadTexture(aiTextureType_NORMALS);
	mat.shininess = loadTexture(aiTextureType_SHININESS);
	mat.opacity = loadTexture(aiTextureType_OPACITY);
}

/**
 * @brief 载入 assimp 网格数据, 创建 Mesh.
 *
 * @param aMesh  assimp 网格.
 * @param aScene assimp 场景.
 * @param path   模型位置.
 * @param meshs  要载入到的 Mesh 数组.
 */
void loadMesh(const aiMesh* aMesh, const aiScene* aScene, const fs::path& path, std::vector<Mesh>& meshs, AABB3& aabb)
{
	static auto currScene = aScene; // TODO: debug
	static unsigned int i = 0;
	if(currScene != aScene)
		currScene = aScene, i = 0;
	printf("Processing Mesh: %3u/%-3u\r", ++i, aScene->mNumMeshes);

	Mesh mesh;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	loadVertices(vertices, aMesh);
	loadIndices(indices, aMesh);

	optimize(indices, vertices);

	VertexFormat format = {
		{"position", Format::RGB32F},
		{"normal", Format::RGB32F},
		{"uv", Format::RG32F},
		{"tangent", Format::RGB32F},
		{"bitangent", Format::RGB32F},
	};

	auto vertexBuffer = VertexBuffer::create(vertices, format);
	auto indexBuffer = IndexBuffer::create(indices);

	const std::string name = aMesh->mName.C_Str();

#if 1
	for(auto& vertex : vertices)
		aabb.expand(vertex.position);
#else
	auto& min = aMesh->mAABB.mMin;
	auto& max = aMesh->mAABB.mMax;
	aabb.expand({{min.x, min.y, min.z}, {max.x, max.y, max.z}});
#endif

	Material mat;

#if 1 // TODO: debug
	loadMaterial(mat, aMesh, aScene, path);
#endif

	mesh.setName(name);
	mesh.setVertexBuffer(vertexBuffer);
	mesh.setIndexBuffer(indexBuffer);
	mesh.setMaterial(mat);

	meshs.push_back(std::move(mesh));
}

/**
 * @brief 从场景中递归载入节点数据.
 *
 * @param aNode  assimp 节点.
 * @param aScene assimp 场景.
 * @param path   模型位置.
 * @param meshs  要载入到的 Mesh 数组.
 */
void loadNode(const aiNode* aNode, const aiScene* aScene, const fs::path& path, std::vector<Mesh>& meshs, AABB3& aabb)
{
	// 加载网格
	for(unsigned int i = 0; i < aNode->mNumMeshes; i++)
		loadMesh(aScene->mMeshes[aNode->mMeshes[i]], aScene, path, meshs, aabb);

	// 加载其余节点
	for(unsigned int i = 0; i < aNode->mNumChildren; i++)
		loadNode(aNode->mChildren[i], aScene, path, meshs, aabb);
}

}

Model::Model()
	: importer(std::make_shared<Assimp::Importer>()),
	exporter(std::make_shared<Assimp::Exporter>())
{
}

void Model::load(const fs::path& path)
{
	if(!fs::exists(path) && !fs::is_regular_file(path))
		throw std::runtime_error("no such file or directory");

	this->path = path;

	Timer timer; // TODO: debug

	// 从文件导入场景数据
	aScene = importer->ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_OptimizeMeshes);

	if(aScene == nullptr || aScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || aScene->mRootNode == nullptr)
		throw std::runtime_error(importer->GetErrorString());

	name = aScene->mName.C_Str();

	printf("Meshes loaded: %.2lfs\n", timer.getSeconds()); // TODO: debug

	timer.restart(); // TODO: debug
	loadNode(aScene->mRootNode, aScene, path, meshs, aabb);
	printf("Meshes processed: %.2lfs\n", timer.getSeconds()); // TODO: debug
}

void Model::loadAsync(const fs::path& path, std::function<void(std::string_view)> callback) noexcept
{
	try
	{
		std::thread([=, this]()
		{
			try
			{
				load(path);
				if(callback)
					callback("");
			}
			catch(std::runtime_error& e)
			{
				if(callback)
					callback(e.what());
			}
		}).detach();
	}
	catch(...)
	{
		assert(false);
	}
}

const AABB3& Model::getAABB() const
{
	return aabb;
}

const std::vector<Mesh> Model::getMeshs() const
{
	return meshs;
}

/*void Model::save(const fs::path& path)
{
	assert(scene != nullptr);

	// 导出场景数据到文件
	auto ret = importer->Export(scene, path.extension().string(), path.string());
	if(ret == aiReturn_FAILURE)
		throw std::runtime_error(importer->GetErrorString());
}

void Model::saveAsync(const fs::path& path, std::function<void(std::string_view)> callback) noexcept
{
	std::thread([=]()
	{
		try
		{
			save(path);
			if(callback)
				callback("");
		} catch(std::runtime_error& e)
		{
			if(callback)
				callback(e.what());
		}
	}).detach();
}*/

// static std::vector<std::future<void>> futures;
// futures.erase(std::remove_if(futures.begin(), futures.end(), [](std::future<void>& f) { return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready; }), futures.end());
