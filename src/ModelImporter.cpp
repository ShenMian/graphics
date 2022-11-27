// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "ModelImporter.h"
#include "Animation.h"
#include "IndexBuffer.h"
#include "Material.h"
#include "Model.h"
#include "VertexBuffer.h"
#include <cassert>

#include "DDSImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace fs = std::filesystem;

namespace
{

std::unordered_map<aiTextureMapMode, Texture::Warp> Warp = {
    {aiTextureMapMode_Wrap, Texture::Warp::Repeat},
    {aiTextureMapMode_Clamp, Texture::Warp::ClampToEdge},
    {aiTextureMapMode_Mirror, Texture::Warp::MirrorRepeat},
    {aiTextureMapMode_Decal, Texture::Warp::ClampToEdge} /* 缺省, 指定一个默认方式 */};

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

Model ModelImporter::load(const fs::path& path, std::function<void(float)> callback)
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
	if(callback)
		importer.SetProgressHandler(new Progress([&](float progress) { callback(progress); }));
	scene = importer.ReadFile(path.string(), fastFlags);

	if(scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr)
		throw std::runtime_error(fmt::format("Assimp: {}", importer.GetErrorString()));

	Model model;
	this->model       = &model;
	this->model->path = path;
	loadScene();

	return model;
}

void ModelImporter::loadScene()
{
	model->name = scene->mName.C_Str();

	for(unsigned int i = 0; i < scene->mNumMaterials; i++)
		loadMaterial(*scene->mMaterials[i]);

	for(unsigned int i = 0; i < scene->mNumAnimations; i++)
		loadAnimation(*scene->mAnimations[i]);

	loadNode(*scene->mRootNode);
}

void ModelImporter::loadNode(const aiNode& node)
{
	for(unsigned int i = 0; i < node.mNumMeshes; i++)
		loadMesh(*scene->mMeshes[node.mMeshes[i]]);

	for(unsigned int i = 0; i < node.mNumChildren; i++)
		loadNode(*node.mChildren[i]);
}

void ModelImporter::loadMesh(const aiMesh& mesh)
{
	const auto vertices = loadVertices(mesh);
	const auto indices  = loadIndices(mesh);

	for(const auto& vertex : vertices)
		model->aabb.expand(vertex.position);

	auto& m = model->materials[mesh.mMaterialIndex];

	model->meshes.push_back({mesh.mName.C_Str(), vertices, indices, &model->materials[mesh.mMaterialIndex]});
	model->vertexCount += vertices.size();
	model->indexCount += indices.size();

	for(unsigned int i = 0; i < mesh.mNumBones; i++)
	{
		auto& bone = mesh.mBones[i];
		bone->mName;
		bone->mOffsetMatrix;
		for(unsigned int j = 0; j < bone->mNumWeights; i++)
		{
			bone->mWeights[i].mVertexId;
			bone->mWeights[i].mWeight;
		}
	}
}

void ModelImporter::loadMaterial(const aiMaterial& mat)
{
	auto loadTexture = [&](aiTextureType type) -> std::shared_ptr<Texture> {
		if(mat.GetTextureCount(type) == 0)
			return nullptr;
		assert(mat.GetTextureCount(type) == 1);

		aiString         path;
		aiTextureMapMode warp[2];
		mat.GetTexture(type, 0, &path, nullptr, nullptr, nullptr, nullptr, warp);
		if(!fs::exists(model->path.parent_path() / path.C_Str()))
			return nullptr;
		if((model->path.parent_path() / path.C_Str()).extension() == ".dds")
		{
			DDSImporter importer;
			return importer.load(model->path.parent_path() / path.C_Str());
		}
		auto texture = Texture::create(model->path.parent_path() / path.C_Str());
		texture->setMinFilter(Texture::Filter::Trilinear);
		texture->setSWarp(Warp[warp[0]]);
		texture->setTWarp(Warp[warp[1]]);
		return texture;
	};

	auto& material = model->materials.emplace_back();
	material.name  = mat.GetName().C_Str();

	aiShadingMode mode; 
	mat.Get(AI_MATKEY_SHADING_MODEL, mode);
	if(mode == aiShadingMode_PBR_BRDF)
	{
		Material::PBR::Workflow workflow;
		float value;
		if(mat.Get(AI_MATKEY_METALLIC_FACTOR, value) == aiReturn_SUCCESS)
		{
			workflow = Material::PBR::Workflow::MetallicRoughness;
		}
		else if(mat.Get(AI_MATKEY_GLOSSINESS_FACTOR, value) == aiReturn_SUCCESS)
		{
			workflow = Material::PBR::Workflow::SpecularGlossiness;
		}
		else
			throw std::runtime_error("unknown PBR workflow");

		material.pbr = {
		    .albedo    = loadTexture(aiTextureType_BASE_COLOR),
		    .normals   = loadTexture(aiTextureType_NORMALS), // aiTextureType_NORMAL_CAMERA
		    .metallic  = loadTexture(aiTextureType_METALNESS),
		    .roughness = loadTexture(aiTextureType_DIFFUSE_ROUGHNESS),
		    .emissive  = loadTexture(aiTextureType_EMISSIVE), // aiTextureType_EMISSION_COLOR
		    .occlusion = loadTexture(aiTextureType_AMBIENT_OCCLUSION),
		    .workflow  = workflow,
		};
	}
	else
	{
		material.diffuse   = loadTexture(aiTextureType_DIFFUSE);
		material.specular  = loadTexture(aiTextureType_SPECULAR);
		material.ambient   = loadTexture(aiTextureType_AMBIENT);
		material.emissive  = loadTexture(aiTextureType_EMISSIVE);
		material.height    = loadTexture(aiTextureType_HEIGHT);
		material.normals   = loadTexture(aiTextureType_NORMALS);
		material.shininess = loadTexture(aiTextureType_SHININESS);
		material.opacity   = loadTexture(aiTextureType_OPACITY);
	}
}

void ModelImporter::loadAnimation(const aiAnimation& anim)
{
	model->animations.emplace_back(anim.mName.C_Str(), static_cast<float>(anim.mDuration / anim.mTicksPerSecond),
	                               static_cast<int>(anim.mTicksPerSecond));
}

std::vector<Mesh::Vertex> ModelImporter::loadVertices(const aiMesh& mesh) noexcept
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

std::vector<unsigned int> ModelImporter::loadIndices(const aiMesh& mesh) noexcept
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
