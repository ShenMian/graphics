// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <math/math.hpp>

/**
 * @brief 视口.
 */
struct Viewport
{
	Viewport() = default;

	Viewport(const Vector2f& size, float minDepth = 0.f, float maxDepth = 1.f)
		: Viewport(0, 0, size.x, size.y, minDepth, maxDepth)
	{
	}

	Viewport(const Vector2f& position, const Vector2f& size, float minDepth = 0.f, float maxDepth = 1.f)
		: Viewport(position.x, position.y, size.x, size.y, minDepth, maxDepth)
	{
	}

	Viewport(float x, float y, float width, float height, float minDepth = 0.f, float maxDepth = 1.f)
		: x(x), y(y), width(width), height(height), minDepth(minDepth), maxDepth(maxDepth)
	{
	}

	float x = 0.f;
	float y = 0.f;
	float width = 0.f;
	float height = 0.f;
	float minDepth = 0.f;
	float maxDepth = 1.f;
};
