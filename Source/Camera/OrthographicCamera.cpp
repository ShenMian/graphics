// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "OrthographicCamera.h"

std::shared_ptr<OrthographicCamera> OrthographicCamera::create(float w, float h, float n, float f)
{
	return std::make_shared<OrthographicCamera>(w, h, n, f);
}

OrthographicCamera::OrthographicCamera(float w, float h, float n, float f)
{
	setProjection(w, h, n, f);
}

void OrthographicCamera::setProjection(float w, float h, float n, float f)
{
	this->width = w;
	this->height = h;
	this->near = n;
	this->far = f;
	projectionDirty = true;
}

float OrthographicCamera::getWidth() const
{
	return width;
}

float OrthographicCamera::getHeight() const
{
	return height;
}

float OrthographicCamera::getNearPlane() const
{
	return near;
}

float OrthographicCamera::getFarPlane() const
{
	return far;
}

Camera::Type OrthographicCamera::getType() const
{
	return Type::Orthographic;
}

void OrthographicCamera::updateProjectionMatrix() const
{
	projection = Matrix4::orthogonal(width, height, near, far);
	projectionDirty = false;
}
