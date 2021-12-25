// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Camera.h"
#include <memory>

class OrthographicCamera : public Camera
{
public:
	static std::shared_ptr<OrthographicCamera> create(float width, float height, float near, float far);

	/**
	 * @brief 构造函数.
	 *
	 * @param width  宽度.
	 * @param height 高度.
	 * @param near   近裁剪平面距离, 最近可见范围.
	 * @param far    远裁剪平面距离, 最远可见范围.
	 *
	 * @see setProjection
	 */
	OrthographicCamera(float width, float height, float near, float far);

	/**
	 * @brief 设置正投影观观察体, 用于正交投影.
	 *
	 * @param width  宽度.
	 * @param height 高度.
	 * @param near   近裁剪平面距离, 最近可见范围.
	 * @param far    远裁剪平面距离, 最远可见范围.
	 */
	void setProjection(float width, float height, float near, float far);

	/**
	 * @brief 获取宽度.
	 */
	float getWidth() const;

	/**
	 * @brief 获取高度.
	 */
	float getHeight() const;

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
	float width;
	float height;
	float near;
	float far;
};
