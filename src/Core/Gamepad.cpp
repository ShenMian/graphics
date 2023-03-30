// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Gamepad.h"
#include "Platform.h"
#include <cassert>
#include <cstring>
#include <fstream>
#include <numeric>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

#if TARGET_OS == OS_WIN
	#include <Windows.h>
	#include <Xinput.h>
#endif

#include <GLFW/glfw3.h>

namespace fs = std::filesystem;

Gamepad::Gamepad(handle_type handle) : handle_(handle)
{
}

Gamepad::~Gamepad()
{
	set_vibration(0.0, 0.0);
}

void Gamepad::update()
{
	// if(!glfwJoystickIsGamepad(handle))
	//     return;

	GLFWgamepadstate state;
	if(!glfwGetGamepadState(handle_, &state))
		return;
	std::memcpy(buttons_, state.buttons, sizeof(buttons_));
	std::memcpy(axes_, state.axes, sizeof(axes_));

	/*
	int axisCount, buttonCount;
	axes = glfwGetJoystickAxes(handle, &axisCount);
	buttons = glfwGetJoystickButtons(handle, &buttonCount);
	assert(axisCount > GLFW_GAMEPAD_AXIS_LAST && buttonCount > GLFW_GAMEPAD_BUTTON_LAST);
	*/
}

Vector2f Gamepad::get(Thumb thumb) const noexcept
{
	Vector2f value = get_raw(thumb);

	float deadzone;
	if(thumb == Thumb::left)
		deadzone = left_thumb_deadzone_;
	else
		deadzone = right_thumb_deadzone_;

	const float factor = 1.f / (1.f - deadzone);

	if(value.norm_sq() > deadzone * deadzone)
	{
		const float magnitude = std::min(value.norm(), 1.f);
		return value.normalized() * ((magnitude - deadzone) * factor);
	}
	else
		return Vector2f(0.f);
}

Vector2f Gamepad::get_raw(Thumb thumb) const noexcept
{
	if(thumb == Thumb::left)
		return {axes_[GLFW_GAMEPAD_AXIS_LEFT_X], axes_[GLFW_GAMEPAD_AXIS_LEFT_Y]};
	else
		return {axes_[GLFW_GAMEPAD_AXIS_RIGHT_X], axes_[GLFW_GAMEPAD_AXIS_RIGHT_Y]};
}

float Gamepad::get(Trigger trigger) const noexcept
{
	const float value  = get_raw(trigger);
	const float factor = 1.f / (1.f - trigger_threshold_);

	if(value > trigger_threshold_)
		return (value - trigger_threshold_) * factor;
	else
		return 0;
}

float Gamepad::get_raw(Trigger trigger) const noexcept
{
	if(trigger == Trigger::left)
		return axes_[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
	else
		return axes_[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
}

bool Gamepad::get(Button button) const noexcept
{
	return buttons_[static_cast<uint8_t>(button)] == GLFW_PRESS;
}

void Gamepad::set_vibration(float leftSpeed, float rightSpeed)
{
#if TARGET_OS == OS_WIN
	if(is_connected())
	{
		XINPUT_VIBRATION state = {};
		state.wLeftMotorSpeed  = leftSpeed * std::numeric_limits<WORD>::max();
		state.wRightMotorSpeed = rightSpeed * std::numeric_limits<WORD>::max();
		if(XInputSetState(handle_, &state) != ERROR_SUCCESS && is_connected())
			throw std::runtime_error("Xinput: failed to set vibration");
	}
#endif
}

std::string_view Gamepad::get_name() const
{
	return glfwGetJoystickName(handle_);
}

bool Gamepad::is_connected() const
{
	return glfwJoystickPresent(handle_); // FIXME: Linux 下未连接手柄时会返回 true
}

bool Gamepad::load_mappings_db(const fs::path& path)
{
	if(!fs::exists(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));
	std::ifstream file(path, std::ios::binary);
	if(!file)
		throw std::runtime_error(fmt::format("failed to open file: {}", path));
	std::stringstream buf;
	buf << file.rdbuf();
	return glfwUpdateGamepadMappings(buf.str().c_str());
}

bool Gamepad::operator==(const Gamepad& rhs) const
{
	return handle_ == rhs.handle_;
}

void Gamepad::init()
{
	glfwSetJoystickCallback([](int handle, int event) {
		if(event == GLFW_CONNECTED)
		{
		}
		else if(event == GLFW_DISCONNECTED)
		{
		}
	});
}

void Gamepad::deinit()
{
	glfwSetJoystickCallback(nullptr);
}
