// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "AssimpImporter.h"
#include "Animation.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Model.h"
#include "VertexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

#include <cassert>

namespace fs = std::filesystem;


namespace
{

// Assimp 文件加载进度回调
class Progress : public Assimp::ProgressHandler
{
public:
	explicit Progress(std::function<void(float)> callback) : callback(std::move(callback)) {}

	bool Update(float percentage) override
	{
		if(callback)
			callback(percentage);
		return true;
	}

private:
	std::function<void(float)> callback;
};

} // namespace

Model AssimpImporter::load(const fs::path& path)
{
	if(!fs::is_regular_file(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	constexpr unsigned int fastFlags =
	    aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_SortByPType |
	    aiProcess_MakeLeftHanded /* TODO */ | aiProcess_GenNormals | aiProcess_GenUVCoords;
	constexpr unsigned int qualityFlags =
	    fastFlags | aiProcess_SplitLargeMeshes | aiProcess_ImproveCacheLocality | aiProcess_FindInvalidData;
	constexpr unsigned int maxQualityFlags = qualityFlags | aiProcess_OptimizeMeshes;

	Assimp::Importer importer;
	importer.SetProgressHandler(
	    new Progress([](float progress) { printf(":: Model loading: %.1f%%  \r", progress * 100); }));
	scene = importer.ReadFile(path.string(), fastFlags);

	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		throw std::runtime_error(fmt::format("Assimp: {}", importer.GetErrorString()));

	Model model;
	this->model = &model;
	loadScene();

	return model;
}

void AssimpImporter::loadScene()
{
	model->name = scene->mName.C_Str();

	for(unsigned int i = 0; i < scene->mNumMaterials; i++)
		loadMaterial(*scene->mMaterials[i]);

	for(unsigned int i = 0; i < scene->mNumAnimations; i++)
		loadAnimation(*scene->mAnimations[i]);

	loadNode(*scene->mRootNode);
}

void AssimpImporter::loadNode(const aiNode& node)
{
	for(unsigned int i = 0; i < node.mNumMeshes; i++)
		loadMesh(*scene->mMeshes[node.mMeshes[i]]);

	for(unsigned int i = 0; i < node.mNumChildren; i++)
		loadNode(*node.mChildren[i]);
}

void AssimpImporter::loadMesh(const aiMesh& mesh)
{
	VertexFormat format = {
	    {"position", Format::RGB32F}, {"normal", Format::RGB32F},    {"uv", Format::RG32F},
	    {"tangent", Format::RGB32F},  {"bitangent", Format::RGB32F},
	};

	const auto vertices = loadVertices(mesh);
	const auto indices  = loadIndices(mesh);

	for(const auto& vertex : vertices)
		model->aabb.expand(vertex.position);

	model->meshs.push_back({mesh.mName.C_Str(), vertices, indices, &model->materials[mesh.mMaterialIndex]});
	model->vertexCount += vertices.size();
	model->indexCount += indices.size();
}

void AssimpImporter::loadMaterial(const aiMaterial& mat)
{
	auto loadTexture = [&](aiTextureType type) -> std::shared_ptr<Texture> {
		if(mat.GetTextureCount(type) == 0)
			return nullptr;
		assert(mat.GetTextureCount(type) == 1);

		aiString name;
		mat.GetTexture(type, 0, &name);
		if(!fs::exists(model->path.parent_path() / name.C_Str()))
			return nullptr;
		return Texture::create(model->path.parent_path() / name.C_Str());
	};

	model->materials.push_back({
	    .name = mat.GetName().C_Str(),
	    .pbr =
	        {
	            .albedo    = loadTexture(aiTextureType_BASE_COLOR),
	            .normals   = loadTexture(aiTextureType_NORMAL_CAMERA),
	            .roughness = loadTexture(aiTextureType_DIFFUSE_ROUGHNESS),
	            .metallic  = loadTexture(aiTextureType_METALNESS),
	            .ao        = loadTexture(aiTextureType_AMBIENT_OCCLUSION),
	            .emissive  = loadTexture(aiTextureType_EMISSION_COLOR),
	        },
	    .diffuse   = loadTexture(aiTextureType_DIFFUSE),
	    .specular  = loadTexture(aiTextureType_SPECULAR),
	    .ambient   = loadTexture(aiTextureType_AMBIENT),
	    .emissive  = loadTexture(aiTextureType_EMISSIVE),
	    .height    = loadTexture(aiTextureType_HEIGHT),
	    .normals   = loadTexture(aiTextureType_NORMALS),
	    .shininess = loadTexture(aiTextureType_SHININESS),
	    .opacity   = loadTexture(aiTextureType_OPACITY),
	});
}

void AssimpImporter::loadAnimation(const aiAnimation& anim)
{
	model->animations.emplace_back(anim.mName.C_Str(), static_cast<float>(anim.mDuration / anim.mTicksPerSecond),
	                               static_cast<int>(anim.mTicksPerSecond));
}

std::vector<Mesh::Vertex> AssimpImporter::loadVertices(const aiMesh& mesh)
{
	std::vector<Mesh::Vertex> vertices;

	for(unsigned int i = 0; i < mesh.mNumVertices; i++)
	{
		auto& vertex = vertices.emplace_back();

		vertex.position.x = mesh.mVertices[i].x;
		vertex.position.y = mesh.mVertices[i].y;
		vertex.position.z = mesh.mVertices[i].z;

		if(mesh.HasNormals()) [[likely]]
		{
			vertex.normal.x = mesh.mNormals[i].x;
			vertex.normal.y = mesh.mNormals[i].y;
			vertex.normal.z = mesh.mNormals[i].z;
		}

		if(mesh.mTextureCoords[0]) [[likely]]
		{
			vertex.uv.x = mesh.mTextureCoords[0][i].x;
			vertex.uv.y = mesh.mTextureCoords[0][i].y;

			vertex.tangent.x = mesh.mTangents[i].x;
			vertex.tangent.y = mesh.mTangents[i].y;
			vertex.tangent.z = mesh.mTangents[i].z;

			vertex.bitangent.x = mesh.mBitangents[i].x;
			vertex.bitangent.y = mesh.mBitangents[i].y;
			vertex.bitangent.z = mesh.mBitangents[i].z;
		}
	}

	return vertices;
}

std::vector<unsigned int> AssimpImporter::loadIndices(const aiMesh& mesh)
{
	std::vector<unsigned int> indices;

	for(unsigned int i = 0; i < mesh.mNumFaces; i++)
	{
		const auto& face = mesh.mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	return indices;
}
