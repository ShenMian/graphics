// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Core/Image.h"
#include "Texture.h"
#include <filesystem>

/**
 * @brief DDS 材质导入器.
 */
class DDSImporter
{
public:
	std::shared_ptr<Texture> load(const std::filesystem::path& path);
};
