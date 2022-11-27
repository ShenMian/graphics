// Copyright 2021 ShenMian
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
	void loadScene();
	void loadNode(const aiNode&);
	void loadMesh(const aiMesh&);
	void loadMaterial(const aiMaterial&);
	void loadAnimation(const aiAnimation&);

	std::vector<Mesh::Vertex> loadVertices(const aiMesh&) noexcept;
	std::vector<unsigned int> loadIndices(const aiMesh&) noexcept;

	const aiScene* scene;
	Model*         model;
};

/** @}*/
