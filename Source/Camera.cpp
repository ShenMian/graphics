// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Camera.h"
#include <cassert>

void Camera::setPerspective(float yFOV, float aspect, float n, float f)
{
	assert(type == Type::Perspective);

	perspective.yFOV = yFOV;
	perspective.aspect = aspect;
	perspective.near = n;
	perspective.far = f;

	projection = Matrix4::perspective(yFOV, aspect, n, f);
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

void Camera::setType(Type type)
{
	this->type = type;
}

Camera::Type Camera::getType() const
{
	return type;
}
