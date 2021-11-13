// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "Texture.h"
#include <string>

/**
 * @brief 材质.
 */
class Material
{
public:
	struct PBR
	{
		std::shared_ptr<Texture> albedo;
		std::shared_ptr<Texture> normals;
		std::shared_ptr<Texture> roughness;
		std::shared_ptr<Texture> metallic;
		std::shared_ptr<Texture> ao;
		std::shared_ptr<Texture> emissive;
	};

	std::string name;

	PBR pbr;

	std::shared_ptr<Texture> diffuse;
	std::shared_ptr<Texture> specular;
	std::shared_ptr<Texture> ambient;
	std::shared_ptr<Texture> emissive;
	std::shared_ptr<Texture> height;
	std::shared_ptr<Texture> normals;
	std::shared_ptr<Texture> shininess;
	std::shared_ptr<Texture> opacity;
};