// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Controller.h"
#include "Camera.h"
#include "Gamepad.h"
#include "Input.h"
#include <algorithm>

Controller::Controller(Camera& cam) : camera_(&cam)
{
}

void Controller::update(float dt)
{
	// FIXME: 同时使用键盘和游戏手柄能达到更高的速度
	process_mouse(dt);
	process_keyboard(dt);
	process_gamepad(dt);

	if(smoothness_ && (camera_->get_position() - target_).norm() > speed_ * 0.001f)
		camera_->set_position(lerp(camera_->get_position(), target_, dt * smoothness_));
	else
		camera_->set_position(target_);
}

void Controller::move_front(float step)
{
	target_ += camera_->get_front() * step;
}

void Controller::move_right(float step)
{
	target_ += camera_->get_right() * step;
}

void Controller::move_up(float step)
{
	const Vector3f unit_y({0.f, 1.f, 0.f}); // TODO
	target_ += -unit_y * step;
}

void Controller::turn_right(float step)
{
	auto rot = camera_->get_rotation();
	rot.y() += step;
	camera_->set_rotation(rot);
}

void Controller::look_up(float step)
{
	auto rot = camera_->get_rotation();
	rot.x() += step;
	rot.x() = std::clamp(rot.x(), -89.f, 89.f);
	camera_->set_rotation(rot);
}

void Controller::set_speed(float v)
{
	speed_ = v;
}

void Controller::set_smoothness(float v)
{
	smoothness_ = v;
}

void Controller::set_camera(Camera& cam)
{
	camera_ = &cam;
	target_ = camera_->get_position();
}

void Controller::set_gamepad(Gamepad& Gamepad)
{
	gamepad_ = &Gamepad;
}

void Controller::process_keyboard(float dt)
{
	float step = speed_ * dt;
	if(Input::is_pressed(Key::LeftShift))
		step *= 3;

	if(Input::is_pressed(Key::W))
		move_front(step);
	if(Input::is_pressed(Key::S))
		move_front(-step);
	if(Input::is_pressed(Key::A))
		move_right(-step);
	if(Input::is_pressed(Key::D))
		move_right(step);
	if(Input::is_pressed(Key::E))
		move_up(step);
	if(Input::is_pressed(Key::Q))
		move_up(-step);
}

void Controller::process_mouse(float dt)
{
	const Vector2f unit({2.f, 1.f}); // TODO
	const Vector2f sensitivity = unit;

	const auto      position = Input::get_mouse_position();
	static Vector2f lastPos  = position;
	const auto      pos      = static_cast<Vector2f>(position);
	Vector2f        offset   = pos - lastPos;
	lastPos                  = pos;

	// TODO
#if 0
	offset.x() *= sensitivity.x() * 8.f * dt;
	offset.y() *= sensitivity.y() * 8.f * dt;
#else
	// 去除 dt 的干扰, 更加稳定
	offset.x() *= sensitivity.x() * 0.09;
	offset.y() *= sensitivity.y() * 0.09;
#endif

	look_up(-offset.y());
	turn_right(offset.x());
}

void Controller::process_gamepad(float dt)
{
	if(!gamepad_->is_connected())
		return;

	gamepad_->update();

	float step = speed_ * dt;
	if(gamepad_->get(Gamepad::Button::LeftThumb))
		step *= 3;

	const Vector2f unit({1.f, 1.f}); // TODO
	const Vector2f sensitivity = unit * 200.f * dt;

	const auto leftThumb    = gamepad_->get(Gamepad::Thumb::left);
	const auto rightThumb   = gamepad_->get(Gamepad::Thumb::right);
	const auto rightTrigger = gamepad_->get(Gamepad::Trigger::right);
	const auto leftTrigger  = gamepad_->get(Gamepad::Trigger::left);
	move_front(-leftThumb.y() * step);
	move_right(leftThumb.x() * step);
	move_up((rightTrigger + (-leftTrigger)) * step);
	look_up(-rightThumb.y() * sensitivity.x());
	turn_right(rightThumb.x() * sensitivity.y());
}
