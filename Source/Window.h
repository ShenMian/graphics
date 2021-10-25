// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <string_view>
#include <filesystem>
#include <functional>

struct Vector2i
{
	int x = 0;
	int y = 0;
};

struct GLFWwindow;

class Window
{
public:
	/**
	 * @brief 构造函数.
	 *
	 * @param title 窗口标题.
	 */
	Window(const std::string& title, Vector2i size);

	/**
	 * @brief 析构函数.
	 */
	virtual ~Window();

	/**
	 * @brief 更新.
	 *
	 * 轮询事件, 响应窗口事件.
	 */
	void update();

	/**
	 * @brief 设置窗口标题.
	 *
	 * @param title 窗口标题.
	 */
	void setTitle(const std::string& title);

	/**
	 * @brief 设置窗口大小.
	 *
	 * @param size 窗口大小.
	 */
	void setSize(Vector2i size);

	/**
	 * @brief 获取窗口大小.
	 */
	Vector2i getSize() const;

	/**
	 * @brief 设置窗口坐标.
	 *
	 * @param pos 新窗口坐标.
	 */
	void setPosition(Vector2i pos);

	/**
	 * @brief 获取窗口坐标.
	 *
	 * @return 当前窗口坐标.
	 */
	Vector2i getPosition();

	/**
	 * @brief 设置窗口可见性.
	 *
	 * @param visible true 为可见, false 为不可见.
	 */
	void setVisible(bool visible);

	/**
	 * @brief 获取窗口可见性.
	 *
	 * @return true 为可见, false 为不可见.
	 */
	bool isVisible() const;

	/**
	 * @brief 设置是否开启垂直同步.
	 *
	 * @param enable true 为开启, false 为关闭.
	 */
	void setSync(bool enable);

	/**
	 * @brief 设置窗口图标.
	 *
	 * @param path 图标文件路径.
	 */
	void setIcon(const std::filesystem::path& path);

	/**
	 * @brief 获取本地句柄.
	 */
	void* getNativeHandle() const;

	std::function<void(Vector2i)>         onResize;    // 窗口大小变化时回调
	std::function<void(bool)>             onFocus;     // 窗口获取/失去焦点回调
	std::function<void()>                 onClose;     // 窗口关闭按钮按下回调
	std::function<void(int, int)>         onKey;       // 键盘按键回调
	std::function<void(double, double)>   onScroll;    // 鼠标滚轮滚动回调
	std::function<void(double, double)>   onMouseMove; // 鼠标移动回调

	static void init();
	static void deinit();

private:
	void setupCallbacks();

	GLFWwindow* handle;
};