// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <filesystem>
#include <unordered_map>
#include <assimp/scene.h>

/**
 * @brief 3D 模型.
 */
class Scene
{
public:
    void load(const std::filesystem::path& path);
    void save(const std::filesystem::path& path);

private:
    aiScene* scene = nullptr;
};

/*
inline static std::vector<std::string> extensions = {
    "collada", "x", "stp", "obj", "objnomtl", "stl", "stlb", "ply", "plyb", "3ds",
    "gltf2", "glb2", "gltf", "glb", "assbin", "assxml", "x3d", "3mf", "pbrt", "assjson"};
*/