// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Camera.h"
#include <cassert>

void Camera::setPosition(Vector3f pos)
{
	position = pos;
	viewDirty = true;
}

Vector3f Camera::getPosition() const
{
	return position;
}

void Camera::updateViewMatrix() const
{
	// TODO
	view = Matrix4::translate(position);
	viewDirty = false;
}

const Matrix4& Camera::getView() const
{
	if(viewDirty)
		updateViewMatrix();
	return view;
}

const Matrix4& Camera::getProjection() const
{
	if(projectionDirty)
		updateProjectionMatrix();
	return projection;
}
