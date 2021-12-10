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

#include <format>
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
void loadVertices(std::vector<Vertex>& vertices, const aiMesh* mesh)
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

		// 获取纹理坐标
		if(mesh->mTextureCoords[0])
			std::memcpy(&vertex.uv, &mesh->mTextureCoords[0][i], sizeof(vertex.uv));

		std::memcpy(&vertex.tangent, &mesh->mTangents[i], sizeof(vertex.tangent));       // 获取 tangent
		std::memcpy(&vertex.bitangent, &mesh->mBitangents[i], sizeof(vertex.bitangent)); // 获取 bitangent

		vertices.push_back(vertex);
	}
}

// 获取索引数据
void loadIndices(std::vector<unsigned int>& indices, const aiMesh* mesh)
{
	// static_assert(std::same_as<decltype(aiFace::mIndices), unsigned int*>);
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

void loadMaterial(Material& mat, const aiMesh* aiMesh, const aiScene* scene, const fs::path path)
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

	mat.name = aiMat->GetName().C_Str();

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
 * @param aiMesh  assimp 网格.
 * @param aiScene assimp 场景.
 * @param path    模型位置.
 * @param meshs   要载入到的 Mesh 数组.
 */
void loadMesh(const aiMesh* aiMesh, const aiScene* aiScene, const fs::path& path, std::vector<Mesh>& meshs)
{
	static auto currScene = aiScene; // TODO: debug
	static unsigned int i = 0;
	if(currScene != aiScene)
		currScene = aiScene, i = 0;
	printf(std::format("处理网格: {:>3}/{:<3}\r", ++i, aiScene->mNumMeshes).c_str());

	Mesh mesh;

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	loadVertices(vertices, aiMesh);
	loadIndices(indices, aiMesh);

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

	const std::string name = aiMesh->mName.C_Str();

	// TODO: 创建包围盒
	// for(auto& vertex : vertices)
	// mesh->mAABB;

	Material mat;
	loadMaterial(mat, aiMesh, aiScene, path);

	mesh.setName(name);
	mesh.setVertexBuffer(vertexBuffer);
	mesh.setIndexBuffer(indexBuffer);
	mesh.setMaterial(mat);

	meshs.push_back(std::move(mesh));
}

/**
 * @brief 从场景中递归载入节点数据.
 *
 * @param aiNode  assimp 节点.
 * @param aiScene assimp 场景.
 * @param path    模型位置.
 * @param meshs   要载入到的 Mesh 数组.
 */
void loadNode(const aiNode* aiNode, const aiScene* aiScene, const fs::path& path, std::vector<Mesh>& meshs)
{
	// 加载网格
	for(unsigned int i = 0; i < aiNode->mNumMeshes; i++)
		loadMesh(aiScene->mMeshes[aiNode->mMeshes[i]], aiScene, path, meshs);

	// 加载其余节点
	for(unsigned int i = 0; i < aiNode->mNumChildren; i++)
		loadNode(aiNode->mChildren[i], aiScene, path, meshs);
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
	aiScene = importer->ReadFile(path.string(),
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if(aiScene == nullptr || aiScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || aiScene->mRootNode == nullptr)
		throw std::runtime_error(importer->GetErrorString());

	name = aiScene->mName.C_Str();

	puts(std::format("网格加载完毕: {}s", timer.getSeconds()).c_str()); // TODO: debug

	timer.restart(); // TODO: debug
	loadNode(aiScene->mRootNode, aiScene, path, meshs);
	puts(std::format("网格处理完毕: {}s", timer.getSeconds()).c_str()); // TODO: debug
}

void Model::loadAsync(const fs::path& path, std::function<void(std::string_view)> callback) noexcept
{
	try
	{
		std::thread([=]()
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
