// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "PerspectiveCamera.h"

std::shared_ptr<PerspectiveCamera> PerspectiveCamera::create(float vFOV, float aspect, float n, float f)
{
	return std::make_shared<PerspectiveCamera>(vFOV, aspect, n, f);
}

PerspectiveCamera::PerspectiveCamera(float vFOV, float aspect, float n, float f)
{
	setProjection(vFOV, aspect, n, f);
}

void PerspectiveCamera::setProjection(float vFOV, float aspect, float n, float f)
{
	this->vFOV = vFOV;
	this->aspectRatio = aspect;
	this->near = n;
	this->far = f;
	projectionDirty = true;
}

float PerspectiveCamera::getVFOV() const
{
	return vFOV;
}

float PerspectiveCamera::getAspect() const
{
	return aspectRatio;
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

void PerspectiveCamera::updateProjectionMatrix() const
{
	projection = Matrix4::perspective(vFOV, aspectRatio, near, far);
	projectionDirty = false;
}
