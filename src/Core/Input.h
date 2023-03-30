// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Key.h"
#include <math/math.hpp>
#include <string_view>

class Window;

/** @addtogroup core
 *  @{
 */

/**
 * @brief 键盘/鼠标输入.
 */
class Input
{
public:
	/**
	 * @brief 获取键盘按键状态.
	 *
	 * @param  key   键盘按键.
	 * @return true  按键按下.
	 * @return false 按键抬起.
	 */
	static bool is_pressed(Key key);

	/**
	 * @brief 获取鼠标按键状态.
	 *
	 * @param  key   键盘按键.
	 * @return true  按键按下.
	 * @return false 按键抬起.
	 */
	static bool is_pressed(Mouse key);

	/**
	 * @brief 获取光标相对窗口的位置.
	 */
	static Vector2 get_mouse_position();

	/**
	 * @brief 通过名称获取键盘按键.
	 *
	 * @param name 键盘按键名称.
	 */
	static Key get_key_by_name(std::string_view name);

	/**
	 * @brief 通过鼠标获取键盘按键.
	 *
	 * @param name 鼠标按键名称.
	 */
	static Mouse get_mouse_by_name(std::string_view name);

	/**
	 * @brief 获取剪切板内容, UTF-8 编码.
	 */
	static std::string_view get_clipboard();

	/**
	 * @brief 设置剪切板内容, UTF-8 编码.
	 *
	 * @param str 要设置的内容.
	 */
	static void set_clipboard(const std::string& str);

	/**
	 * @brief 设置接受输入的窗口.
	 */
	static void set_window(const Window& win);

private:
	static const Window* window;
};

/** @}*/
