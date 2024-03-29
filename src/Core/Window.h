// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Key.h"
#include <filesystem>
#include <functional>
#include <math/math.hpp>
#include <string>

struct GLFWwindow;

class Image;

/** @addtogroup core
 *  @{
 */

/**
 * @brief 窗体.
 */
class Window
{
public:
	/**
	 * @brief 构造函数, 创建一个窗口.
	 *
	 * @param title      窗口标题.
	 * @param size       窗口大小(分辨率).
	 * @param fullscreen 是否全屏, true 为全屏.
	 */
	Window(std::string_view title, const Vector2i& size);

	/**
	 * @brief 析构函数.
	 */
	virtual ~Window();

	/**
	 * @brief 更新.
	 *
	 * 轮询窗口事件, 并交换缓冲区.
	 */
	void update();

	/**
	 * @brief 设置窗口标题.
	 *
	 * @param title 窗口标题.
	 */
	void set_title(std::string_view title);

	/**
	 * @brief 设置窗口大小.
	 *
	 * @param size 窗口大小.
	 */
	void set_size(const Vector2i& size);

	/**
	 * @brief 获取窗口大小.
	 */
	Vector2i get_size() const;

	/**
	 * @brief 设置窗口坐标.
	 *
	 * @param pos 新窗口坐标.
	 */
	void set_position(const Vector2i& pos);

	/**
	 * @brief 获取窗口坐标.
	 *
	 * @return 当前窗口坐标.
	 */
	Vector2i get_position() const;

	/**
	 * @brief 设置窗口可见性.
	 *
	 * @param visible true 为可见, false 为不可见.
	 */
	void set_visible(bool visible);

	/**
	 * @brief 获取窗口可见性.
	 *
	 * @return true 为可见, false 为不可见.
	 */
	bool is_visible() const noexcept;

	/**
	 * @brief 设置窗口是否全屏.
	 *
	 * @param fullscreen true 为全屏, false 为窗口.
	 */
	void set_fullscreen(bool fullscreen);

	/**
	 * @brief 是否处于全屏模式.
	 */
	bool is_fullscreen() const noexcept;

	/**
	 * @brief 设置是否开启垂直同步.
	 *
	 * @param enable true 为开启, false 为关闭.
	 */
	void set_vsync(bool enable) noexcept;

	/**
	 * @brief 设置大小是否可调.
	 *
	 * @param enable true 为可调, false 为不可调.
	 */
	void set_resizable(bool enable);

	/**
	 * @brief 是否可调窗口大小.
	 */
	bool is_resizable() const noexcept;

	/**
	 * @brief 设置窗口是否置顶.
	 *
	 * @param enable true 为置顶, false 为不置顶.
	 */
	void set_floating(bool enable);

	/**
	 * @brief 是否已置顶.
	 */
	bool is_floating() const noexcept;

	/**
	 * @brief 设置窗口图标.
	 *
	 * @param image 图标图像.
	 */
	void set_icon(const Image& image);

	/**
	 * @brief 请求聚焦.
	 */
	void request_focus();

	/**
	 * @brief 设置是否开启光标锁定.
	 *
	 * 会将光标隐藏并锁定在窗口中.
	 *
	 * @param enable true 为开启, false 为关闭.
	 */
	void set_cursor_lock(bool enable);

	/**
	 * @brief 是否开启光标锁定.
	 */
	bool is_cursor_lock() const;

	/**
	 * @brief 设置光标在窗口中的坐标.
	 *
	 * @param pos 新光标在窗口中的坐标.
	 */
	void set_cursor_position(const Vector2i& pos);

	/**
	 * @brief 获取光标在窗口中的坐标.
	 */
	Vector2d get_cursor_position() const;

	/**
	 * @brief 设置是否启用原始鼠标运动.
	 *
	 * @param enable true 为开启, false 为关闭.
	 *
	 * 原始鼠标运动更接近鼠标在表面上的实际运动. 它不受应用于桌面光标运动的缩放和加速度的影响. 适合控制 3D 相机.
	 *
	 * @warning 仅在禁用光标时有效.
	 */
	void set_raw_mouse_motion(bool enable);

	/**
	 * @brief 获取本地句柄.
	 */
	GLFWwindow* get_handle() const;

	std::function<void(Vector2i)>                 onResize;     ///< 窗口大小变化时回调
	std::function<void(bool)>                     onFocus;      ///< 窗口获取/失去焦点回调
	std::function<void()>                         onClose;      ///< 窗口关闭按钮按下回调
	std::function<void(int, Key)>                 onKey;        ///< 键盘按键回调
	std::function<void(Vector2d)>                 onScroll;     ///< 鼠标滚轮滚动回调
	std::function<void(Vector2d)>                 onMouseMove;  ///< 鼠标移动回调
	std::function<void(bool)>                     onMouseEnter; ///< 鼠标进入窗口回调
	std::function<void(int, Mouse)>               onMouse;      ///< 鼠标按键回调
	std::function<void(int, const char* paths[])> onDrop;       ///< 路径拽入窗口回调

	static void init();
	static void deinit();

private:
	void setup_callbacks();

	GLFWwindow* handle_;

	Vector2i size_;
	Vector2i position_;
};

/** @}*/
