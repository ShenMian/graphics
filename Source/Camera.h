// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <Math/Math.hpp>

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
	 * @param vFOV        垂直视角范围, 单位: 弧度.
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

	/**
	 * @brief 获取垂直视角范围, 单位: 弧度.
	 */
	float getVFOV() const;

	/**
	 * @brief 获取水平视角范围, 单位: 弧度.
	 */
	float getHFOV() const;

	/**
	 * @brief 获取宽高比.
	 */
	float getAspectRatio() const;

	/**
	 * @brief 获取近裁剪平面距离.
	 */
	float getNear() const;

	/**
	 * @brief 获取远裁剪平面距离.
	 */
	float getFar() const;

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
