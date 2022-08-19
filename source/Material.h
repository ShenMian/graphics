// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Texture.h"
#include <string>

/** @addtogroup model
 *  @{
 */

/**
 * @brief 材质.
 */
struct Material
{
	struct PBR
	{
		std::shared_ptr<Texture> albedo;
		std::shared_ptr<Texture> normals;   ///< 法线.
		std::shared_ptr<Texture> metallic;  ///< 金属.
		std::shared_ptr<Texture> roughness; ///< 粗糙.
		std::shared_ptr<Texture> emissive;
		std::shared_ptr<Texture> occlusion; ///< 环境光遮蔽.
	};

	std::string name;

	PBR pbr;

	std::shared_ptr<Texture> diffuse;   ///< 漫反射.
	std::shared_ptr<Texture> specular;  ///< 镜面反射.
	std::shared_ptr<Texture> ambient;   ///< 环境光.
	std::shared_ptr<Texture> emissive;
	std::shared_ptr<Texture> height;
	std::shared_ptr<Texture> normals;   ///< 法线.
	std::shared_ptr<Texture> shininess; ///< 镜面反射指数.
	std::shared_ptr<Texture> opacity;
};

/** @}*/
