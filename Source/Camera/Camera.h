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

protected:
	Matrix4 view;
	Matrix4 projection;
};
