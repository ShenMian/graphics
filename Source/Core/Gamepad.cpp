// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Gamepad.h"
#include <cassert>
#include <GLFW/glfw3.h>

Gamepad::Gamepad()
{
	glfwSetJoystickCallback([](int handle, int event){
		switch(event)
		{
		case GLFW_CONNECTED:
			break;

		case GLFW_DISCONNECTED:
			break;
		}
	});
}

void Gamepad::update()
{
	axes = glfwGetJoystickAxes(handle, &axisCount);
	buttons = glfwGetJoystickButtons(handle, &buttonCount);

	assert(axisCount >= 6 && buttonCount >= 14);
}

bool Gamepad::get(Button button) const
{
	return buttons[static_cast<uint8_t>(button)] == GLFW_PRESS;
}

Vector2 Gamepad::get(Thumb thumb) const
{
	switch(thumb)
	{
	case Thumb::left:
		return {axes[0], axes[1]};
		break;

	case Thumb::right:
		return {axes[2], axes[3]};
		break;
	}
}

float Gamepad::get(Trigger trigger) const
{
	switch(trigger)
	{
	case Trigger::left:
		return axes[4];
		break;

	case Trigger::right:
		return axes[5];
		break;
	}
}

std::string_view Gamepad::getName() const
{
	return glfwGetJoystickName(handle);
}
