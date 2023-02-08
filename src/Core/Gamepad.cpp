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

Gamepad::Gamepad(handle_type handle) : handle(handle)
{
}

Gamepad::~Gamepad()
{
	setVibration(0.0, 0.0);
}

void Gamepad::update()
{
	// if(!glfwJoystickIsGamepad(handle))
	//     return;

	GLFWgamepadstate state;
	if(!glfwGetGamepadState(handle, &state))
		return;
	std::memcpy(buttons, state.buttons, sizeof(buttons));
	std::memcpy(axes, state.axes, sizeof(axes));

	/*
	int axisCount, buttonCount;
	axes = glfwGetJoystickAxes(handle, &axisCount);
	buttons = glfwGetJoystickButtons(handle, &buttonCount);
	assert(axisCount > GLFW_GAMEPAD_AXIS_LAST && buttonCount > GLFW_GAMEPAD_BUTTON_LAST);
	*/
}

Vector2f Gamepad::get(Thumb thumb) const noexcept
{
	Vector2f value = getRaw(thumb);

	float deadzone;
	if(thumb == Thumb::left)
		deadzone = leftThumbDeadzone;
	else
		deadzone = rightThumbDeadzone;

	const float factor = 1.f / (1.f - deadzone);

	if(value.normSq() > deadzone * deadzone)
	{
		const float magnitude = std::min(value.norm(), 1.f);
		return value.normalized() * ((magnitude - deadzone) * factor);
	}
	else
		return Vector2f::zero;
}

Vector2f Gamepad::getRaw(Thumb thumb) const noexcept
{
	assert(thumb == Thumb::left || thumb == Thumb::right);

	if(thumb == Thumb::left)
		return {axes[GLFW_GAMEPAD_AXIS_LEFT_X], axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};
	else
		return {axes[GLFW_GAMEPAD_AXIS_RIGHT_X], axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]};
}

float Gamepad::get(Trigger trigger) const noexcept
{
	const float value  = getRaw(trigger);
	const float factor = 1.f / (1.f - triggerThreshold);

	if(value > triggerThreshold)
		return (value - triggerThreshold) * factor;
	else
		return 0;
}

float Gamepad::getRaw(Trigger trigger) const noexcept
{
	assert(trigger == Trigger::left || trigger == Trigger::right);

	if(trigger == Trigger::left)
		return axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];
	else
		return axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];
}

bool Gamepad::get(Button button) const noexcept
{
	return buttons[static_cast<uint8_t>(button)] == GLFW_PRESS;
}

void Gamepad::setVibration(float leftSpeed, float rightSpeed)
{
#if TARGET_OS == OS_WIN
	if(isConnected())
	{
		XINPUT_VIBRATION state = {};
		state.wLeftMotorSpeed  = leftSpeed * std::numeric_limits<WORD>::max();
		state.wRightMotorSpeed = rightSpeed * std::numeric_limits<WORD>::max();
		if(XInputSetState(handle, &state) != ERROR_SUCCESS && isConnected())
			throw std::runtime_error("Xinput: failed to set vibration");
	}
#endif
}

std::string_view Gamepad::getName() const
{
	return glfwGetJoystickName(handle);
}

bool Gamepad::isConnected() const
{
	return glfwJoystickPresent(handle); // FIXME: Linux 下未连接手柄时会返回 true
}

bool Gamepad::loadMappingsDb(const fs::path& path)
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
	return handle == rhs.handle;
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
