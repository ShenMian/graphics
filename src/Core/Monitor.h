// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <math/math.hpp>
#include <string>
#include <vector>

struct GLFWmonitor;

/** @addtogroup core
 *  @{
 */

/**
 * @brief 显示器.
 */
class Monitor
{
public:
	/**
	 * @brief 获取主显示器.
	 */
	static const Monitor* get_primary() noexcept;

	/**
	 * @brief 获取全部显示器.
	 */
	static const std::vector<Monitor>& get_monitors();

	/**
	 * @brief 获取显示器名称.
	 */
	std::string_view get_name() const;

	/**
	 * @brief 获取显示器大小(即分辨率), 单位: 像素.
	 */
	Vector2i get_size() const;

	/**
	 * @brief 获取显示器位置.
	 */
	Vector2i get_position() const;

	/**
	 * @brief 获取显示器刷新率, 单位: Hz.
	 */
	int get_refresh_rate() const;

	/**
	 * @brief 是否为主显示器.
	 */
	bool is_primary() const;

	/**
	 * @brief 获取本地句柄.
	 */
	GLFWmonitor* get_handle() const;

	bool operator==(const Monitor&) const = default;

	static void init();
	static void deinit();

private:
	Monitor() = default;
	Monitor(GLFWmonitor* handle);

	GLFWmonitor* handle_ = nullptr;

	static std::vector<Monitor> monitors_;
};

/** @}*/
