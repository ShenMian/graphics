// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <Math/Math.hpp>

// TODO: up 不会变, 导致不能正确的绕 z 轴旋转, 旋转后 getRight 会使用不正确的 up 进行计算

/**
 * @brief 摄像机.
 */
class Camera
{
public:
	/**
	 * @brief 投影类型.
	 */
	enum class Type
	{
		Perspective, // 透视投影
		Orthographic // 正交投影
	};

	/**
	 * @brief 默认构造函数.
	 *
	 * @param type 投影类型.
	 */
	Camera(Type type);

	/**
	 * @brief 设置截头锥体观察体, 用于透视投影.
	 *
	 * @param vFOV        垂直视角范围, 单位: 弧度制.
	 * @param aspectRatio 宽高比.
	 * @param near        近裁剪平面距离, 最近可见范围.
	 * @param far         远裁剪平面距离, 最远可见范围.
	 */
	void setPerspective(float vFOV, float aspectRatio, float near, float far);

	/**
	 * @brief 设置正投影观观察体, 用于正交投影.
	 *
	 * @param width  宽度.
	 * @param height 高度.
	 * @param near   近裁剪平面距离, 最近可见范围.
	 * @param far    远裁剪平面距离, 最远可见范围.
	 */
	void setOrthographic(float width, float height, float near, float far);

	/**
	 * @brief 获取观察矩阵.
	 */
	const Matrix4& getView() const;

	/**
	 * @brief 获取投影矩阵.
	 */
	const Matrix4& getProjection() const;

	/**
	 * @brief 获取投影类型.
	 */
	Type getType() const;

	void setPosition(Vector3f pos);
	Vector3f getPosition() const;

	void setRotation(Vector3f rot);
	Vector3f getRotation() const;

	Vector3f getFront() const;
	Vector3f getRight() const;
	Vector3f getUp() const;

protected:
	void updateProjectionMatrix() const;
	void updateViewMatrix() const;

	struct Perspective
	{
		float vFOV;
		float aspectRatio;
		float near;
		float far;
	};

	struct Orthographic
	{
		float width;
		float height;
		float near;
		float far;
	};

	union
	{
		Perspective  perspective;
		Orthographic orthographic;
	};

	Type type;

	Vector3f position;
	Vector3f rotation;
	Vector3f up = -Vector3f::unit_y;

	mutable Matrix4 projection;
	mutable bool    projectionDirty = true;
	mutable Matrix4 view;
	mutable bool    viewDirty = true;
};
