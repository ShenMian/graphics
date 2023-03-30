// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Mesh.h"
#include <filesystem>
#include <functional>

/** @addtogroup model
 *  @{
 */

class Mesh;
class Model;
class Vector;
class Animation;
struct Material;

struct aiScene;
struct aiNode;
struct aiMesh;
struct aiMaterial;
struct aiAnimation;

/**
 * @brief Assimp 模型导入器.
 */
class ModelImporter
{
public:
	/**
	 * @brief 加载模型.
	 * 
	 * @param path     模型文件路径.
	 * @param callback 加载进度回调.
	 */
	Model load(const std::filesystem::path& path, std::function<void(float)> callback = nullptr);

private:
	void load_scene();
	void load_node(const aiNode&);
	void load_mesh(const aiMesh&);
	void load_material(const aiMaterial&);
	void load_animation(const aiAnimation&);

	std::vector<Mesh::Vertex> load_vertices(const aiMesh&) noexcept;
	std::vector<unsigned int> load_indices(const aiMesh&) noexcept;

	const aiScene* scene_;
	Model*         model_;
};

/** @}*/
