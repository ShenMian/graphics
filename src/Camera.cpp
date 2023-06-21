// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Camera.h"
#include <cassert>

Camera::Camera(Type type) : type_(type)
{
}

void Camera::set_perspective(float vFOV, float aspectRatio, float near, float far)
{
	assert(type_ == Type::Perspective);
	perspective_.vFOV        = vFOV;
	perspective_.aspectRatio = aspectRatio;
	perspective_.near        = near;
	perspective_.far         = far;
	projection_dirty_         = true;
}

void Camera::set_orthographic(float width, float height, float near, float far)
{
	assert(type_ == Type::Orthographic);
	orthographic_.width  = width;
	orthographic_.height = height;
	orthographic_.near   = near;
	orthographic_.far    = far;
	projection_dirty_     = true;
}

Camera::Type Camera::get_type() const
{
	return type_;
}

void Camera::set_position(const Vector3f& pos)
{
	position_  = pos;
	view_dirty_ = true;
}

const Vector3f& Camera::get_position() const
{
	return position_;
}

void Camera::set_rotation(const Vector3f& rot)
{
	rotation_  = rot;
	view_dirty_ = true;
}

const Vector3f& Camera::get_rotation() const
{
	return rotation_;
}

Vector3f Camera::get_front() const
{
	Vector3f front;
	front.x() = -std::cos(radians(rotation_.x())) * std::sin(radians(rotation_.y()));
	front.y() = -std::sin(radians(rotation_.x()));
	front.z() = std::cos(radians(rotation_.x())) * std::cos(radians(rotation_.y()));
	front.normalize();
	return {front.x(), front.y(), -front.z()};
}

Vector3f Camera::get_right() const
{
	return get_front().cross(up_).normalize();
}

Vector3f Camera::get_up() const
{
	return get_front().cross(get_right());
}

float Camera::get_vfov() const
{
	if(type_ == Type::Perspective)
		return perspective_.vFOV;
	else
		return std::atan(orthographic_.height / (orthographic_.far - orthographic_.near)) * 2.f;
}

float Camera::get_hfov() const
{
	return std::atan(std::tan(get_vfov() / 2.f) * get_aspect_ratio()) * 2.f;
	;
}

float Camera::get_aspect_ratio() const
{
	if(type_ == Type::Perspective)
		return perspective_.aspectRatio;
	else
		return orthographic_.width / orthographic_.height;
}

float Camera::get_near() const
{
	return perspective_.near;
}

float Camera::get_far() const
{
	return perspective_.far;
}

const Matrix4& Camera::get_view() const
{
	update_view_matrix();
	return view_;
}

const Matrix4& Camera::get_projection() const
{
	update_projection_matrix();
	return projection_;
}

void Camera::update_projection_matrix() const
{
	if(type_ == Type::Perspective)
		projection_ = Matrix4::perspective(perspective_.vFOV, perspective_.aspectRatio, perspective_.near, perspective_.far);
	else
		projection_ = Matrix4::orthogonal(orthographic_.width, orthographic_.height, orthographic_.near, orthographic_.far);
	projection_dirty_ = false;
}

void Camera::update_view_matrix() const
{
	// TODO

	auto trans = Matrix4f::create_translate(position_);
	auto rot   = Matrix4f::identity();
#if 0
	rot *= Matrix4f::create_rotation(radians(rotation.x()), Vector3f::unit_x);
	rot *= Matrix4f::create_rotation(radians(-rotation.y()), Vector3f::unit_y);
	rot *= Matrix4f::create_rotation(radians(rotation.z()), Vector3f::unit_z);
#else
	rot *= Matrix4f::create_rotation_x(radians(rotation_.x()));
	rot *= Matrix4f::create_rotation_y(radians(rotation_.y()));
	rot *= Matrix4f::create_rotation_z(radians(rotation_.z()));
#endif
	view_ = rot * trans;

	view_dirty_ = false;
}
