﻿// Copyright 2021 ShenMian
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
     * @brief 设置截头锥体观察体, 用于透视投影.
     *
     * @param yFOV   垂直视角范围, 单位: 弧度制.
     * @param aspect 宽高比.
     * @param near   近平面距离.
     * @param far    远平面距离.
     */
    void setPerspective(float yFOV, float aspect, float near, float far);

    /**
     * @brief 设置正投影观观察体, 用于正交投影.
     *
     * @param width  宽度.
     * @param height 高度.
     * @param near   近平面距离.
     * @param far    远平面距离.
     */
    void setOrthographic(float width, float height, float near, float far);

    /**
     * @brief 设置投影类型.
     */
	void setType(Type type);

    /**
     * @brief 获取投影类型.
     */
    Type getType() const;

private:
	union
	{
		struct
		{
			float yFOV;
			float aspect;
			float near;
			float far;
		} perspective;

		struct
		{
			float width;
			float height;
			float near;
			float far;
		} orthographic;
	};

	Matrix4 projection;
	Type    type = Type::Perspective;;
};