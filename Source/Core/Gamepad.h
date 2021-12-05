// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <string_view>
#include <Math/Vector2.hpp>

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
	Gamepad();

	/**
	 * @brief 更新数据.
	 */
	void update();

	/**
	 * @brief 获取摇杆数据.
	 *
	 * @param thumb 摇杆.
	 * @return Vector2 范围: [-1, 1]
	 */
	Vector2 get(Thumb thumb) const;

	/**
	 * @brief 获取线性按键数据.
	 *
	 * @param trigger 线性按键.
	 * @return float 范围: [-1, 1]
	 */
	float get(Trigger trigger) const;

	/**
	 * @brief 获取按键状态.
	 *
	 * @param  button 按键.
	 * @return true   按键按下.
	 * @return false  按键抬起.
	 */
	bool get(Button button) const;

	/**
	 * @brief 获取设备名称.
	 */
	std::string_view getName() const;

private:
	handle_type handle = 0;

	int          axisCount = 0;
	const float* axes = nullptr;
	int                  buttonCount = 0;
	const unsigned char* buttons = nullptr;
};

enum class Gamepad::Thumb
{
	left,
	right
};

enum class Gamepad::Trigger
{
	left,
	right
};

enum class Gamepad::Button : uint8_t
{
	A = 0,
	B = 1,
	X = 2,
	Y = 3,

	LeftBumper = 4,
	RightBumper = 5,

	Back = 6,
	Start = 7,
	Guide = 8,

	LeftThumb = 9,
	RightThumb = 10,

	DPAD_Up = 11,
	DPAD_Right = 12,
	DPAD_Down = 13,
	DPAD_Left = 14,

	Cross = A,
	Circle = B,
	Square = X,
	Triangle = Y
};
