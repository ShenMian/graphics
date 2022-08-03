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
#include <assimp/ProgressHandler.hpp>

#include <algorithm>
#include <cstring>
#include <future>
#include <meshoptimizer.h>
#include <vector>

#include "Core/Clock.hpp"

namespace fs = std::filesystem;

namespace
{

// Assimp 文件加载进度回调
class Progress : public Assimp::ProgressHandler
{
public:
	explicit Progress(std::function<void(float)> callback)
		: callback(std::move(callback))
	{
	}

	bool Update(float percentage) override
	{
		if(callback)
			callback(percentage);
		return true;
	}

private:
	std::function<void(float)> callback;
};

// 获取顶点数据
template <typename Vertex>
void loadVertices(std::vector<Vertex>& vertices, const aiMesh* mesh)
{
	static_assert(std::same_as<ai_real, float>);

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex = {};

		// 获取坐标
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		// 获取法向量
		if(mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if(mesh->mTextureCoords[0])
		{
			vertex.uv.x = mesh->mTextureCoords[0][i].x; // 获取纹理坐标
			vertex.uv.y = mesh->mTextureCoords[0][i].y;

			vertex.tangent.x = mesh->mTangents[i].x; // 获取 tangent
			vertex.tangent.y = mesh->mTangents[i].y;
			vertex.tangent.z = mesh->mTangents[i].z;

			vertex.bitangent.x = mesh->mBitangents[i].x; // 获取 bitangent
			vertex.bitangent.y = mesh->mBitangents[i].y;
			vertex.bitangent.z = mesh->mBitangents[i].z;
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

// 获取顶点骨骼数据
template <typename Vertex>
void loadBones(std::vector<Vertex>& vertices, std::vector<Matrix4f>& bones, const aiMesh* mesh)
{
	bones.resize(mesh->mNumBones);
	for(unsigned int boneId = 0; boneId < mesh->mNumBones; boneId++)
	{
		const auto& bone = *mesh->mBones[boneId];
		bones[boneId] = Matrix4f(bone.mOffsetMatrix[0]);

		/*
		for(unsigned int i = 0; i < bone.mNumWeights; i++)
		{
			const auto& weight = bone.mWeights[i];
			for(size_t j = 0; j < vertices[weight.mVertexId].bones.size(); j++)
			{
				if(vertices[weight.mVertexId].weights[j] == 0)
				{
					vertices[weight.mVertexId].bones[j] = boneId;
					vertices[weight.mVertexId].weights[j] = weight.mWeight;
					break;
				}
			}
		}
		*/
	}
}

// 加载材质
void loadMaterial(Material& mat, const aiMesh* mesh, const aiScene* scene, const fs::path& path)
{
	const auto dir = path.parent_path();
	const auto aMat = scene->mMaterials[mesh->mMaterialIndex];

	// 加载指定类型的材质
	auto loadTexture = [&](aiTextureType type)->std::shared_ptr<Texture>
	{
		// TODO: 此处循环无用, 同类型的材质可能有多个
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
 * @param scene  assimp 场景.
 * @param path   模型位置.
 * @param meshs  要载入到的 Mesh 数组.
 */
void loadMesh(const aiMesh* mesh, const fs::path& path, std::vector<Mesh>& meshes, std::vector<Matrix4f>& bones, AABB3& aabb)
{
	std::vector<Mesh::Vertex> vertices;
	std::vector<unsigned int> indices;

	// TODO: 应该先加载全部的材质, 然后通过 ID 获取材质
	Material material;

	// 读取模型数据
	loadVertices(vertices, mesh);
	loadIndices(indices, mesh);
	loadBones(vertices, bones, mesh);
	// loadMaterial(material, mesh, scene, path);

	const std::string name = mesh->mName.C_Str();

	for(const auto& vertex : vertices)
		aabb.expand(vertex.position);

	meshes.emplace_back(name, vertices, indices, &material);
}

/**
 * @brief 从场景中递归载入节点数据.
 *
 * @param node   assimp 节点.
 * @param scene  assimp 场景.
 * @param path   模型文件位置.
 * @param meshes 要载入到的 Mesh 数组.
 */
void loadNode(const aiNode* node, const aiScene* scene, const fs::path& path, std::vector<Mesh>& meshes, std::vector<Matrix4f>& bones, AABB3& aabb)
{
	// 加载网格
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		printf("Processing Mesh: %3u/%-3u\r", i, scene->mNumMeshes); // TODO: debug
		loadMesh(scene->mMeshes[node->mMeshes[i]], path, meshes, bones, aabb);
	}

	// 加载其余节点
	for(unsigned int i = 0; i < node->mNumChildren; i++)
		loadNode(node->mChildren[i], scene, path, meshes, bones, aabb);
}

}

void Model::load(const fs::path& path, unsigned int process, std::function<void(float)> progress)
{
	if(!fs::exists(path) && !fs::is_regular_file(path))
		throw std::runtime_error("no such file or directory");

	this->path = path;
	name.clear();
	meshes.clear();
	aabb.clear();

	Clock clock; // TODO: debug

	unsigned int flags = aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_SortByPType |
		aiProcess_MakeLeftHanded; // TODO

	constexpr unsigned int fastFlags = aiProcess_GenNormals | aiProcess_GenUVCoords;
	constexpr unsigned int qualityFlags = fastFlags | aiProcess_SplitLargeMeshes | aiProcess_ImproveCacheLocality | aiProcess_FindInvalidData;
	constexpr unsigned int maxQualityFlags = qualityFlags | aiProcess_OptimizeMeshes;

	switch(process)
	{
	case ProcessFlags::Fast:
		flags |= fastFlags;
		break;

	case ProcessFlags::Quality:
		flags |= qualityFlags;
		break;

	case ProcessFlags::MaxQuality:
		flags |= maxQualityFlags;
		break;
	}

	Assimp::Importer importer;
	auto progressHandler = new Progress(progress);
	importer.SetProgressHandler(progressHandler);
	const aiScene* scene = importer.ReadFile(path.string(), flags); // 从文件导入场景数据

	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		throw std::runtime_error(importer.GetErrorString());

	name = scene->mName.C_Str();

	// TODO: debug
	{
		printf("Meshes loaded: %.2lfs       \n", clock.getSeconds());
		clock.restart();
		loadNode(scene->mRootNode, scene, path, meshes, bones, aabb);
		for(const auto& mesh : meshes)
			meshInfo += mesh.getInfo();
		printf("Meshes processed: %.2lfs       \n", clock.getSeconds());
	}

	for(unsigned int i = 0; i < scene->mNumAnimations; i++)
	{
		const auto& anim = *scene->mAnimations[i];
		animations.emplace(anim.mName.C_Str(), Animation(anim.mName.C_Str(), anim.mDuration / anim.mTicksPerSecond, (int)anim.mTicksPerSecond));
	}
}

const std::string& Model::getName() const
{
	return name;
}

const AABB3& Model::getAABB() const
{
	return aabb;
}

const std::vector<Mesh>& Model::getMeshes() const
{
	return meshes;
}

const Animation& Model::getAnimation(const std::string& name) const
{
	return animations.at(name);
}

const Mesh::Info& Model::getMeshInfo() const
{
	return meshInfo;
}

void Model::compress()
{
	for(auto& mesh : meshes)
		mesh.compress();
}

void Model::decompress()
{
	for(auto& mesh : meshes)
		mesh.decompress();
}
