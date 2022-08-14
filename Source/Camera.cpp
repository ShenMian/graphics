// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Camera.h"

Camera::Camera(Type type) : type(type)
{
}

void Camera::setPerspective(float vFOV, float aspectRatio, float near, float far)
{
	assert(type == Type::Perspective);
	perspective.vFOV        = vFOV;
	perspective.aspectRatio = aspectRatio;
	perspective.near        = near;
	perspective.far         = far;
	projectionDirty         = true;
}

void Camera::setOrthographic(float width, float height, float near, float far)
{
	assert(type == Type::Orthographic);
	orthographic.width  = width;
	orthographic.height = height;
	orthographic.near   = near;
	orthographic.far    = far;
	projectionDirty     = true;
}

const Matrix4& Camera::getView() const
{
	updateViewMatrix();
	return view;
}

const Matrix4& Camera::getProjection() const
{
	updateProjectionMatrix();
	return projection;
}

Camera::Type Camera::getType() const
{
	return type;
}

void Camera::setPosition(const Vector3f& pos)
{
	position  = pos;
	viewDirty = true;
}

const Vector3f& Camera::getPosition() const
{
	return position;
}

void Camera::setRotation(const Vector3f& rot)
{
	rotation  = rot;
	viewDirty = true;
}

const Vector3f& Camera::getRotation() const
{
	return rotation;
}

Vector3f Camera::getFront() const
{
	Vector3f front;
	front.x = -std::cos(radians(rotation.x)) * std::sin(radians(rotation.y));
	front.y = -std::sin(radians(rotation.x));
	front.z = std::cos(radians(rotation.x)) * std::cos(radians(rotation.y));
	front.normalize();
	return {front.x, front.y, -front.z};
}

Vector3f Camera::getRight() const
{
	return getFront().cross(up).normalize();
}

Vector3f Camera::getUp() const
{
	return getFront().cross(getRight());
}

float Camera::getVFOV() const
{
	if(type == Type::Perspective)
		return perspective.vFOV;
	else
		return std::atan(orthographic.height / (orthographic.far - orthographic.near)) * 2.f;
}

float Camera::getHFOV() const
{
	return std::atan(std::tan(getVFOV() / 2.f) * getAspectRatio()) * 2.f;
	;
}

float Camera::getAspectRatio() const
{
	if(type == Type::Perspective)
		return perspective.aspectRatio;
	else
		return orthographic.width / orthographic.height;
}

float Camera::getNear() const
{
	return perspective.near;
}

float Camera::getFar() const
{
	return perspective.far;
}

void Camera::updateProjectionMatrix() const
{
	if(type == Type::Perspective)
		projection = Matrix4::perspective(perspective.vFOV, perspective.aspectRatio, perspective.near, perspective.far);
	else
		projection = Matrix4::orthogonal(orthographic.width, orthographic.height, orthographic.near, orthographic.far);
	projectionDirty = false;
}

void Camera::updateViewMatrix() const
{
	// TODO

	auto trans = Matrix4f::createTranslate(position);
	auto rot   = Matrix4f::identity();
#if 0
	rot *= Matrix4f::createRotation(radians(rotation.x), Vector3f::unit_x);
	rot *= Matrix4f::createRotation(radians(-rotation.y), Vector3f::unit_y);
	rot *= Matrix4f::createRotation(radians(rotation.z), Vector3f::unit_z);
#else
	rot *= Matrix4f::createRotationX(radians(rotation.x));
	rot *= Matrix4f::createRotationY(radians(rotation.y));
	rot *= Matrix4f::createRotationZ(radians(rotation.z));
#endif
	view = rot * trans;

	viewDirty = false;
}
