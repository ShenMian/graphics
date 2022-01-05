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

void Camera::setDirection(Vector3f dir)
{
	direction = dir.normalized();
	viewDirty = true;
}

Vector3f Camera::getDirection() const
{
	return direction;
}

Vector3f Camera::getFront() const
{
	return direction.normalized();
}

Vector3f Camera::getRight() const
{
	return direction.cross(up).normalize();
}

Vector3f Camera::getUp() const
{
	return up;
}

void Camera::updateViewMatrix() const
{
	view = Matrix4::lookAt(position, position + direction, up);
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
