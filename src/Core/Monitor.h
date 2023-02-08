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
	static const Monitor* getPrimary() noexcept;

	/**
	 * @brief 获取全部显示器.
	 */
	static const std::vector<Monitor>& getMonitors();

	/**
	 * @brief 获取显示器名称.
	 */
	std::string_view getName() const;

	/**
	 * @brief 获取显示器大小(即分辨率), 单位: 像素.
	 */
	Vector2i getSize() const;

	/**
	 * @brief 获取显示器位置.
	 */
	Vector2i getPosition() const;

	/**
	 * @brief 获取显示器刷新率, 单位: Hz.
	 */
	int getRefreshRate() const;

	/**
	 * @brief 是否为主显示器.
	 */
	bool isPrimary() const;

	/**
	 * @brief 获取本地句柄.
	 */
	GLFWmonitor* getHandle() const;

	bool operator==(const Monitor&) const = default;

	static void init();
	static void deinit();

private:
	Monitor() = default;
	Monitor(GLFWmonitor* handle);

	GLFWmonitor* handle = nullptr;

	static std::vector<Monitor> monitors;
};

/** @}*/
