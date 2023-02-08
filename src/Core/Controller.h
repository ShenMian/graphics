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

	void moveFront(float step);
	void moveRight(float step);
	void moveUp(float step);

	void turnRight(float step);
	void lookUp(float step);

	/**
	 * @brief 设置移动速度.
	 *
	 * @param v 移动速度.
	 */
	void setSpeed(float v);

	/**
	 * @brief 设置平滑度.
	 *
	 * @param v 平滑度, 越高缓冲效果时间越短. 0 表示不开启平滑.
	 */
	void setSmoothness(float v);

	/**
	 * @brief 设置控制的相机.
	 *
	 * @param cam 控制的相机.
	 */
	void setCamera(Camera& cam);

	/**
	 * @brief 设置要获取输入的手柄.
	 *
	 * @param gamepad 手柄.
	 */
	void setGamepad(Gamepad& gamepad);

private:
	/**
	 * @brief 处理键盘输入, 控制相机移动.
	 *
	 * @param dt 时间变化量.
	 */
	void processKeyboard(float dt);

	/**
	 * @brief 处理鼠标输入, 控制相机转向.
	 *
	 * @param dt 时间变化量.
	 */
	void processMouse(float dt);

	/**
	 * @brief 处理手柄输入, 控制相机移动和转向.
	 *
	 * @param dt 时间变化量.
	 */
	void processGamepad(float dt);

	Camera* camera = nullptr;
	float   speed  = 200.f;

	float    smoothness = 11;
	Vector3f target;

	Gamepad* gamepad = nullptr;
};
