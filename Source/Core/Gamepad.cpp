// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Gamepad.h"
#include <cassert>
#include <cstring>
#include <GLFW/glfw3.h>

Gamepad::Gamepad(handle_type handle)
	: handle(handle)
{
}

void Gamepad::update()
{
	assert(glfwJoystickIsGamepad(handle));

	GLFWgamepadstate state;
	glfwGetGamepadState(handle, &state);
	std::memcpy(buttons, state.buttons, sizeof(buttons));
	std::memcpy(axes, state.axes, sizeof(axes));

	/*
	int axisCount, buttonCount;
	axes = glfwGetJoystickAxes(handle, &axisCount);
	buttons = glfwGetJoystickButtons(handle, &buttonCount);
	assert(axisCount > GLFW_GAMEPAD_AXIS_LAST && buttonCount > GLFW_GAMEPAD_BUTTON_LAST);
	*/
}

bool Gamepad::get(Button button) const
{
	return buttons[static_cast<uint8_t>(button)] == GLFW_PRESS;
}

Vector2f Gamepad::get(Thumb thumb) const
{
	Vector2f value = getRaw(thumb);

	float deadzone;
	if(thumb == Thumb::left)
		deadzone = leftThumbDeadzone;
	else
		deadzone = rightThumbDeadzone;

	if(value.normSquared() > deadzone * deadzone)
		return value.normalized() * (value.norm() - deadzone);
	else
		return Vector2f::zero;
}

Vector2f Gamepad::getRaw(Thumb thumb) const
{
	switch(thumb)
	{
	case Thumb::left:
		return {axes[GLFW_GAMEPAD_AXIS_LEFT_X], axes[GLFW_GAMEPAD_AXIS_LEFT_Y]};

	case Thumb::right:
		return {axes[GLFW_GAMEPAD_AXIS_RIGHT_X], axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]};

	default:
		assert(false);
		return {};
	}
}

float Gamepad::get(Trigger trigger) const
{
	float value = getRaw(trigger);

	if(value > triggerThreshold)
		return value - triggerThreshold;
	else
		return 0;
}

float Gamepad::getRaw(Trigger trigger) const
{
	switch(trigger)
	{
	case Trigger::left:
		return axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER];

	case Trigger::right:
		return axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER];

	default:
		assert(false);
		return {};
	}
}

std::string_view Gamepad::getName() const
{
	return glfwGetJoystickName(handle);
}

bool Gamepad::isConnected() const
{
	return glfwJoystickPresent(handle);
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
