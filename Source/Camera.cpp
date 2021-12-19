// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Camera.h"
#include <cassert>

void Camera::setPerspective(float vFOV, float aspect, float n, float f)
{
	assert(type == Type::Perspective);

	perspective.yFOV = vFOV;
	perspective.aspect = aspect;
	perspective.near = n;
	perspective.far = f;

	projection = Matrix4::perspective(vFOV, aspect, n, f);
}

void Camera::setOrthographic(float w, float h, float n, float f)
{
	assert(type == Type::Orthographic);

	orthographic.width = w;
	orthographic.height = h;
	orthographic.near = n;
	orthographic.far = f;

	projection = Matrix4::orthogonal(w, h, n, f);
}

const Matrix4& Camera::getView() const
{
	return view;
}

const Matrix4& Camera::getProjection() const
{
	return projection;
}

void Camera::setType(Type type)
{
	this->type = type;
}

Camera::Type Camera::getType() const
{
	return type;
}
