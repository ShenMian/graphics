// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <math/math.hpp>

class Camera;
class Gamepad;

/**
 * @brief 摄像机控制器.
 */
class Controller
{
public:
	/**
	 * @brief 默认构造函数.
	 */
	Controller() = default;

	/**
	 * @brief 构造函数.
	 *
	 * @param cam 控制的相机.
	 */
	Controller(Camera& cam);

	/**
	 * @brief 通过外部输入更新相机.
	 *
	 * @param dt 时间变化量.
	 */
	void update(float dt);

	void move_front(float step);
	void move_right(float step);
	void move_up(float step);

	void turn_right(float step);
	void look_up(float step);

	/**
	 * @brief 设置移动速度.
	 *
	 * @param v 移动速度.
	 */
	void set_speed(float v);

	/**
	 * @brief 设置平滑度.
	 *
	 * @param v 平滑度, 越高缓冲效果时间越短. 0 表示不开启平滑.
	 */
	void set_smoothness(float v);

	/**
	 * @brief 设置控制的相机.
	 *
	 * @param cam 控制的相机.
	 */
	void set_camera(Camera& cam);

	/**
	 * @brief 设置要获取输入的手柄.
	 *
	 * @param gamepad 手柄.
	 */
	void set_gamepad(Gamepad& gamepad);

private:
	/**
	 * @brief 处理键盘输入, 控制相机移动.
	 *
	 * @param dt 时间变化量.
	 */
	void process_keyboard(float dt);

	/**
	 * @brief 处理鼠标输入, 控制相机转向.
	 *
	 * @param dt 时间变化量.
	 */
	void process_mouse(float dt);

	/**
	 * @brief 处理手柄输入, 控制相机移动和转向.
	 *
	 * @param dt 时间变化量.
	 */
	void process_gamepad(float dt);

	Camera* camera_ = nullptr;
	float   speed_  = 200.f;

	float    smoothness_ = 11;
	Vector3f target_;

	Gamepad* gamepad_ = nullptr;
};
