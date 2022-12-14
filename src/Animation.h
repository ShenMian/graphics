// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>

/**
 * @brief 3D 动画.
 */
class Animation
{
public:
	Animation(const std::string& name, float duration, uint32_t tickRate);

	/**
	 * @brief 获取名称.
	 */
	const std::string& getName() const;

	/**
	 * @brief 获取持续时间, 单位: 秒.
	 */
	float getDuration() const;

	uint32_t getTickRate() const;

private:
	std::string name;
	float       duration;
	uint32_t    tickRate;
};
