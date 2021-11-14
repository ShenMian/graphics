// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <system_error>
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <functional>
#include <unordered_map>

// TODO: 析构后线程可能仍在运行, 导致 callback 获取不存在的数据.
//       或许应该让用户自己创建一个线程/协程来执行同步操作.

class Mesh;
class Material;

/**
 * @brief 3D 模型.
 */
class Model
{
public:
	/**
	 * @brief 从文件载入场景.
	 * 
	 * @param path 场景文件路径.
	 * 
	 * @see loadAsync
	 */
	void load(const std::filesystem::path& path);

	/**
	 * @brief 从文件载入场景, 异步.
	 *
	 * @param path 场景文件路径.
	 *
	 * @see load
	 */
	void loadAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback = nullptr) noexcept;

	/**
	 * @brief 保存场景到文件.
	 *
	 * @param path 场景文件路径.
	 */
	// void save(const std::filesystem::path& path);
	// void saveAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback = nullptr) noexcept;

	const std::vector<Mesh> getMeshs() const;

private:
	/**
	 * @brief 从场景中递归载入节点数据.
	 * 
	 * @param node assimp 节点.
	 */
	void loadNode(aiNode* node);

	/**
	 * @brief 载入 assimp 网格数据, 创建 Mesh.
	 * 
	 * @param mesh assimp 网格.
	 */
	void loadMesh(aiMesh* mesh);

	/**
	 * @brief 载入 assimp 材质数据到 Mesh 的 Material.
	 *
	 * @param mesh 网格.
	 * @param aiMesh assimp 网格.
	 */
	void loadMaterial(Mesh* mesh, aiMesh* aiMesh);

	std::string           name;
	std::vector<Mesh>     meshs;
	std::filesystem::path path;

	const aiScene*   scene = nullptr;
	Assimp::Importer importer;
	Assimp::Exporter exporter;
};

/*
inline static std::vector<std::string> extensions = {
	"collada", "x", "stp", "obj", "objnomtl", "stl", "stlb", "ply", "plyb", "3ds",
	"gltf2", "glb2", "gltf", "glb", "assbin", "assxml", "x3d", "3mf", "pbrt", "assjson"};
*/