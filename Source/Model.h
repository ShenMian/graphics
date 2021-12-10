// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <stdexcept>
#include <filesystem>
#include <functional>
#include <unordered_map>

// TODO:
// 1. 顶点数据应该由 Model 管理, Mesh 保存顶点数据的引用和顶点索引.
// 2. 析构后线程可能仍在运行, 导致 callback 获取不存在的数据.

namespace Assimp
{

class Importer;
class Exporter;

}

struct aiScene;

class Mesh;
class Material;

/**
 * @brief 3D 模型.
 */
class Model
{
public:
	Model();

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
	void loadAsync(const std::filesystem::path& path, std::function<void(std::string_view)> callback = nullptr) noexcept;

	/**
	 * @brief 保存场景到文件.
	 *
	 * @param path 场景文件路径.
	 */
	 // void save(const std::filesystem::path& path);
	 // void saveAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback = nullptr) noexcept;

	const std::vector<Mesh> getMeshs() const;

private:
	std::string           name;
	std::vector<Mesh>     meshs;
	std::filesystem::path path;

	const aiScene* aiScene = nullptr;
	std::shared_ptr<Assimp::Importer> importer;
	std::shared_ptr<Assimp::Exporter> exporter;
};

/*
inline static std::vector<std::string> extensions = {
	"collada", "x", "stp", "obj", "objnomtl", "stl", "stlb", "ply", "plyb", "3ds",
	"gltf2", "glb2", "gltf", "glb", "assbin", "assxml", "x3d", "3mf", "pbrt", "assjson"};
*/
