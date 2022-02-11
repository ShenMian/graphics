// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Mesh.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <Math/Math.hpp>
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
			GenNormals = 1 << 0,       ///< 生成法向量.
			GenSmoothNormals = 1 << 1, ///< 生成平滑的法向量
			GenTexCoords = 1 << 2,     ///< 生成纹理坐标.

			RemoveRedundantMaterials = 1 << 3, ///< 移除冗余的材质.
			SplitLargeMeshes = 1 << 4,         ///< 分割大的网格数据为若干子网格.
			OptimizeMeshes = 1 << 5,           ///< 减少网格数量.
			ImproveCacheLocality = 1 << 6,     ///< 重新排序三角形以获得更好的顶点缓存数据局部性.

			FixInfacingNormals = 1 << 7, ///< 查找指向内部的法向量, 并反转他们.
			FindInvalidData = 1 << 8,    ///< 查找无效的数据.

			/// 快速.
			Fast = GenNormals | GenTexCoords,

			/// 质量.
			Quality = GenSmoothNormals | GenTexCoords | SplitLargeMeshes | ImproveCacheLocality | FindInvalidData,

			/// 最高质量.
			MaxQuality = Quality | OptimizeMeshes
		};
	};

	/**
	 * @brief 从文件载入场景.
	 *
	 * @param path 场景文件路径.
	 *
	 * @see loadAsync
	 */
	void load(const std::filesystem::path& path, unsigned int process = ProcessFlags::Fast, std::function<void(float)> progress = nullptr);

	/**
	 * @brief 从文件载入场景, 异步.
	 *
	 * @param path 场景文件路径.
	 *
	 * @see load
	 */
	void loadAsync(const std::filesystem::path& path, unsigned int process = ProcessFlags::Fast, std::function<void(std::string_view)> callback = nullptr) noexcept;

	/**
	 * @brief 保存场景到文件.
	 *
	 * @param path 场景文件路径.
	 */
	 // void save(const std::filesystem::path& path);
	 // void saveAsync(const std::filesystem::path& path, std::function<void(std::error_code)> callback = nullptr) noexcept;

	const AABB3& getAABB() const;

	const std::vector<Mesh>& getMeshs() const;

	void compress();
	void decompress();

private:
	std::string           name;
	std::vector<Mesh>     meshes;
	std::filesystem::path path;
	AABB3                 aabb;

	const aiScene* aScene = nullptr;
};

/*
inline static std::vector<std::string> extensions = {
	"collada", "x", "stp", "obj", "objnomtl", "stl", "stlb", "ply", "plyb", "3ds",
	"gltf2", "glb2", "gltf", "glb", "assbin", "assxml", "x3d", "3mf", "pbrt", "assjson"};
*/

/** @}*/
