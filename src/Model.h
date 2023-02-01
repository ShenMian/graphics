// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Animation.h"
#include "Material.h"
#include "Mesh.h"
#include <math/math.hpp>
#include <string>

/** @addtogroup model
 *  @{
 */

/**
 * @brief 3D 模型.
 */
struct Model
{
	std::string            name;
	std::vector<Mesh>      meshes;
	std::vector<Material>  materials;
	std::vector<Animation> animations;
	std::filesystem::path  path;

	AABB aabb;

	uint32_t vertexCount = 0;
	uint32_t indexCount  = 0;

	void load(const std::filesystem::path& path);
	void save(const std::filesystem::path& path);
};

/** @}*/
