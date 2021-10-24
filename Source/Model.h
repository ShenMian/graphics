// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "ErrorCode.h"
#include <assimp/scene.h>
#include <assimp/Exporter.hpp>
#include <assimp/Importer.hpp>
#include <filesystem>
#include <functional>
#include <system_error>
#include <unordered_map>

// 1. 析构后线程可能仍在运行, 导致 callback 获取不存在的数据.

/**
 * @brief 3D 模型.
 */
class Model
{
public:
	void load(const std::filesystem::path& path, std::error_code& ec = ErrorCode);
	void loadAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback);

	void save(const std::filesystem::path& path, std::error_code& ec = ErrorCode);
	void saveAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback);

private:
	void loadNode(aiNode* node);
	void loadMesh(aiMesh* mesh);

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