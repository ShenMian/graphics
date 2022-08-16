// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Model.h"
#include <filesystem>

/** @addtogroup model
 *  @{
 */

/**
 * @brief 模型导入器.
 */
class Importer
{
public:
	virtual Model load(const std::filesystem::path& path) = 0;
};

/** @}*/
