// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <functional>
#include <math/math.hpp>
#include <string_view>

/** @addtogroup core
 *  @{
 */

/**
 * @brief 游戏手柄.
 *
 * @warning 不支持震动.
 */
class Gamepad
{
public:
	using handle_type = unsigned short;

	enum class Thumb;
	enum class Trigger;
	enum class Button : uint8_t;

	/**
	 * @brief 默认构造函数.
	 */
	Gamepad(handle_type handle = 0);

	/**
	 * @brief 更新数据.
	 */
	void update();

	/**
	 * @brief 获取摇杆数据.
	 *
	 * @param thumb 摇杆.
	 * @return Vector2f 范围: [-1, 1]
	 */
	Vector2f get(Thumb thumb) const noexcept;

	/**
	 * @brief 获取原始摇杆数据.
	 *
	 * @param thumb 摇杆.
	 * @return Vector2f 范围: [-1, 1]
	 * @warning 不建议直接使用.
	 */
	Vector2f getRaw(Thumb thumb) const noexcept;

	/**
	 * @brief 获取线性按键力度.
	 *
	 * @param trigger 线性按键.
	 * @return float 0 表示没有按下, 范围: [0, 1]
	 */
	float get(Trigger trigger) const noexcept;

	/**
	 * @brief 获取原始线性按键数据.
	 *
	 * @param trigger 线性按键.
	 * @return float 0 表示没有按下, 范围: [0, 1]
	 * @warning 不建议直接使用.
	 */
	float getRaw(Trigger trigger) const noexcept;

	/**
	 * @brief 获取按键状态.
	 *
	 * @param  button 按键.
	 * @return true   按键按下.
	 * @return false  按键抬起.
	 */
	bool get(Button button) const noexcept;

	/**
	 * @brief 获取设备名称. 人类可读的, UTF-8 编码.
	 */
	std::string_view getName() const;

	/**
	 * @brief 检查手柄是否处于连接状态.
	 *
	 * @return true  已连接.
	 * @return false 已断开.
	 */
	bool isConnected() const;

	bool operator==(const Gamepad&) const;

	static void init();
	static void deinit();

private:
	handle_type handle;

	float leftThumbDeadzone  = 0.1f;
	float rightThumbDeadzone = 0.1f;
	float triggerThreshold   = 0.01f;

	unsigned char buttons[15] = {};
	float         axes[6]     = {};
};

/**
 * @brief 手柄摇杆.
 */
enum class Gamepad::Thumb
{
	left,
	right
};

/**
 * @brief 手柄线性按键.
 */
enum class Gamepad::Trigger
{
	left, ///< 左侧线性按键, 即 LT.
	right ///< 右侧线性按键, 即 RT.
};

/**
 * @brief 手柄按键.
 */
enum class Gamepad::Button : uint8_t
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,

	LeftBumper  = 4,
	RightBumper = 5,

	Back  = 6,
	Start = 7,
	Guide = 8,

	LeftThumb  = 9,
	RightThumb = 10,

	DPAD_Up    = 11,
	DPAD_Right = 12,
	DPAD_Down  = 13,
	DPAD_Left  = 14,

	Cross    = A,
	Circle   = B,
	Square   = X,
	Triangle = Y
};

/** @}*/

/**
 * @class Gamepad
 *
 * @details 获取手柄的按键状态, 摇杆数据和设备名称.
 *
 * ```cpp
 * Gamepad gamepad; // 如果使用一个手柄, 可以直接使用默认构造函数
 *
 * if(!gamepad.isConnected()) // 检测手柄是否处于连接状态
 * 	return;
 *
 * gamepad.update(); // 更新手柄数据, 获取按键/摇杆数据前需要先更新
 *
 * // 获取手柄按键/摇杆数据的成员函数名称是 get, 但是它们的返回值是不一样的.
 * bool     pressed   = gamepad.get(Gamepad::Button::A))     // 获取按键 A 是否按下
 * float    magnitude = gamepad.get(Gamepad::Trigger::left); // 获取左侧线性按键(LT)按力度
 * Vector2f position  = gamepad.get(Gamepad::Thumb::left);   // 获取左侧摇杆数据
 * ```
 */
