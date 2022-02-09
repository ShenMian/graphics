// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Input.h"
#include "Window.h"
#include <GLFW/glfw3.h>

const Window* Input::window;

bool Input::isPressed(Key key)
{
	auto* native = static_cast<GLFWwindow*>(window->getNativeHandle());
	return glfwGetKey(native, static_cast<int>(key)) == GLFW_PRESS;
}

bool Input::isPressed(Mouse key)
{
	auto* native = static_cast<GLFWwindow*>(window->getNativeHandle());
	return glfwGetMouseButton(native, static_cast<int>(key)) == GLFW_PRESS;
}

Vector2 Input::getMousePosition()
{
	Vector2d pos;
	auto* native = static_cast<GLFWwindow*>(window->getNativeHandle());
	glfwGetCursorPos(native, &pos.x, &pos.y);
	return Vector2(pos);
}

float Input::getMouseX()
{
	return getMousePosition().x;
}

float Input::getMouseY()
{
	return getMousePosition().y;
}

std::string_view Input::getClipboard()
{
	return glfwGetClipboardString(nullptr);
}

void Input::setClipboard(const std::string& str)
{
	glfwSetClipboardString(nullptr, str.c_str());
}

void Input::setWindow(const Window& win)
{
	window = &win;
}
