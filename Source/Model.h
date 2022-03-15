// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Mesh.h"
#include <Math/Math.hpp>
#include <stdexcept>
#include <filesystem>
#include <functional>
#include <unordered_map>

class Mesh;
class Material;

/** @addtogroup model
 *  @{
 */

 /**
  * @brief 3D 模型.
  */
class Model
{
public:
	struct ProcessFlags
	{
		enum
		{
			Fast,      /// 快速.
			Quality,   /// 质量.
			MaxQuality /// 最高质量.
		};
	};

	/**
	 * @brief 从文件载入场景.
	 *
	 * @param path     模型文件路径.
	 * @param process  要进行的处理.
	 * @param progress 载入进度回调.
	 *
	 * @see loadAsync
	 */
	void load(const std::filesystem::path& path, unsigned int process = ProcessFlags::Fast, std::function<void(float)> progress = nullptr);

	/**
	 * @brief 获取名称.
	 */
	[[nodiscard]] const std::string& getName() const;

	/**
	 * @brief 获取 AABB.
	 */
	[[nodiscard]] const AABB3& getAABB() const;

    /**
     * @brief 获取网格.
     */
    [[nodiscard]] const std::vector<Mesh>& getMeshes() const;

	/**
	 * @brief 获取网格信息.
	 */
	[[nodiscard]] const Mesh::Info& getMeshInfo() const;

	void compress();
	void decompress();

private:
	std::string           name;
	std::vector<Mesh>     meshes;
	std::filesystem::path path;
	AABB3                 aabb;
	Mesh::Info            meshInfo;
};

/*
inline static std::vector<std::string> extensions = {
	"collada", "x", "stp", "obj", "objnomtl", "stl", "stlb", "ply", "plyb", "3ds",
	"gltf2", "glb2", "gltf", "glb", "assbin", "assxml", "x3d", "3mf", "pbrt", "assjson"};
*/

/** @}*/
