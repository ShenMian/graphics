// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Camera.h"

class PerspectiveCamera : public Camera
{
public:
	/**
	 * @brief 构造函数.
	 *
	 * @param vFOV   垂直视角范围, 单位: 弧度制.
	 * @param aspect 宽高比.
	 * @param near   近裁剪平面距离, 最近可见范围.
	 * @param far    远裁剪平面距离, 最远可见范围.
	 *
	 * @see setProjection
	 */
	PerspectiveCamera(float vFOV, float aspect, float near, float far);

	/**
	 * @brief 设置截头锥体观察体, 用于透视投影.
	 *
	 * @param vFOV   垂直视角范围, 单位: 弧度制.
	 * @param aspect 宽高比.
	 * @param near   近裁剪平面距离, 最近可见范围.
	 * @param far    远裁剪平面距离, 最远可见范围.
	 */
	void setProjection(float vFOV, float aspect, float near, float far);

	/**
	 * @brief 获取垂直视角范围, 单位: 弧度制.
	 */
	float getVFOV() const;

	/**
	 * @brief 获取宽高比.
	 */
	float getAspect() const;

	/**
	 * @brief 获取近裁剪平面.
	 */
	float getNearPlane() const;

	/**
	 * @brief 获取远裁剪平面.
	 */
	float getFarPlane() const;

	Type getType() const override;

private:
	float vFOV;
	float aspect;
	float near;
	float far;
};
