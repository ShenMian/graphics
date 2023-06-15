// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <math/math.hpp>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief 骨骼.
 */
struct Bone
{
	std::string           name;
	Matrix4f              offset;
	std::vector<uint32_t> indices;
	std::vector<float>    weights;
};
