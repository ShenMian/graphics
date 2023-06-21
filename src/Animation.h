// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>

/**
 * @brief 3D 动画.
 */
struct Animation
{
	Animation(const std::string& name, float duration, uint32_t tick_rate)
	    : name(name), duration(duration), tick_rate(tick_rate)
	{
	}

	std::string name;
	float       duration;
	uint32_t    tick_rate;
};
