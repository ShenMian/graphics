﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Mesh.h"
#include <filesystem>

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
	Model load(const std::filesystem::path& path);

private:
	void loadScene();
	void loadNode(const aiNode&);
	void loadMesh(const aiMesh&);
	void loadMaterial(const aiMaterial&);
	void loadAnimation(const aiAnimation&);

	std::vector<Mesh::Vertex> loadVertices(const aiMesh&);
	std::vector<unsigned int> loadIndices(const aiMesh&);

	const aiScene* scene;
	Model*         model;
};

/** @}*/