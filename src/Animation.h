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
	Animation(const std::string& name, float duration, uint32_t tick_rate);

	/**
	 * @brief 获取名称.
	 */
	const std::string& get_name() const;

	/**
	 * @brief 获取持续时间, 单位: 秒.
	 */
	float get_duration() const;

	uint32_t get_tick_rate() const;

private:
	std::string name_;
	float       duration_;
	uint32_t    tick_rate_;
};
