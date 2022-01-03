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
	virtual Type getType() const = 0;

	void setPosition(Vector3f pos);
	Vector3f getPosition() const;

	Vector3f getFront() const;
	Vector3f getRight() const;
	Vector3f getUp() const;

protected:
	/**
	 * @brief 更新投影矩阵, 并清除脏标记.
	 */
	virtual void updateProjectionMatrix() const = 0;

	mutable Matrix4 projection;
	mutable bool    projectionDirty = true;

private:
	/**
	 * @brief 更新视图矩阵, 并清除脏标记.
	 */
	void updateViewMatrix() const;

	Vector3f position;
	Vector3f direction = -Vector3f::unit_z;
	Vector3f up = -Vector3f::unit_y;

	mutable Matrix4 view;
	mutable bool    viewDirty = true;
};
