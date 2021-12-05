// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Key.h"
#include <Math/Vector2.hpp>

class Window;

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
	static bool isPressed(Key key);

	/**
	 * @brief 获取鼠标按键状态.
	 *
	 * @param  key   键盘按键.
	 * @return true  按键按下.
	 * @return false 按键抬起.
	 */
	static bool isPressed(Mouse key);

	/**
	 * @brief 获取光标相对窗口的位置.
	 */
	static Vector2 getMousePosition();

	/**
	 * @brief 获取光标相对窗口的 X 坐标.
	 */
	static float getMouseX();

	/**
	 * @brief 获取光标相对窗口的 Y 坐标.
	 */
	static float getMouseY();

	/**
	 * @brief 设置接受输入的窗口.
	 */
	static void setWindow(Window* win);

private:
	static Window* window;
};
