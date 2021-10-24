// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "ErrorCode.h"
#include <assimp/scene.h>
#include <filesystem>
#include <functional>
#include <system_error>
#include <unordered_map>

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
	aiScene* scene = nullptr;
};

/*
inline static std::vector<std::string> extensions = {
	"collada", "x", "stp", "obj", "objnomtl", "stl", "stlb", "ply", "plyb", "3ds",
	"gltf2", "glb2", "gltf", "glb", "assbin", "assxml", "x3d", "3mf", "pbrt", "assjson"};
*/