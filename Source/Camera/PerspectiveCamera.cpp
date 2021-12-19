// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(float vFOV, float aspect, float n, float f)
{
	setProjection(vFOV, aspect, n, f);
}

void PerspectiveCamera::setProjection(float vFOV, float aspect, float n, float f)
{
	this->vFOV = vFOV;
	this->aspect = aspect;
	this->near = n;
	this->far = f;

	projection = Matrix4::perspective(vFOV, aspect, n, f);
}

float PerspectiveCamera::getVFOV() const
{
	return vFOV;
}

float PerspectiveCamera::getAspect() const
{
	return aspect;
}

float PerspectiveCamera::getNearPlane() const
{
	return near;
}

float PerspectiveCamera::getFarPlane() const
{
	return far;
}

Camera::Type PerspectiveCamera::getType() const
{
	return Type::Perspective;
}
