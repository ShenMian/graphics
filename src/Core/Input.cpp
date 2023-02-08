// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Input.h"
#include "Window.h"
#include <GLFW/glfw3.h>
#include <unordered_map>

namespace
{

const std::unordered_map<std::string_view, Key> nameToKey = {{"Unknown", Key::Unknown},
                                                             {"Space", Key::Space},
                                                             {"Apostrophe", Key::Apostrophe},
                                                             {"Comma", Key::Comma},
                                                             {"Minus", Key::Minus},
                                                             {"Period", Key::Period},
                                                             {"Slash", Key::Slash},
                                                             {"0", Key::_0},
                                                             {"1", Key::_1},
                                                             {"2", Key::_2},
                                                             {"3", Key::_3},
                                                             {"4", Key::_4},
                                                             {"5", Key::_5},
                                                             {"6", Key::_6},
                                                             {"7", Key::_7},
                                                             {"8", Key::_8},
                                                             {"9", Key::_9},
                                                             {"Semicolon", Key::Semicolon},
                                                             {"Equal", Key::Equal},
                                                             {"A", Key::A},
                                                             {"B", Key::B},
                                                             {"C", Key::C},
                                                             {"D", Key::D},
                                                             {"E", Key::E},
                                                             {"F", Key::F},
                                                             {"G", Key::G},
                                                             {"H", Key::H},
                                                             {"I", Key::I},
                                                             {"J", Key::J},
                                                             {"K", Key::K},
                                                             {"L", Key::L},
                                                             {"M", Key::M},
                                                             {"N", Key::N},
                                                             {"O", Key::O},
                                                             {"P", Key::P},
                                                             {"Q", Key::Q},
                                                             {"R", Key::R},
                                                             {"S", Key::S},
                                                             {"T", Key::T},
                                                             {"U", Key::U},
                                                             {"V", Key::V},
                                                             {"W", Key::W},
                                                             {"X", Key::X},
                                                             {"Y", Key::Y},
                                                             {"Z", Key::Z},
                                                             {"LeftBracket", Key::Left},
                                                             {"Backslash", Key::Backslash},
                                                             {"RightBracket", Key::RightBracket},
                                                             {"GraveAccent", Key::GraveAccent},
                                                             {"Escape", Key::Escape},
                                                             {"Enter", Key::Enter},
                                                             {"Tab", Key::Tab},
                                                             {"Backspace", Key::Backspace},
                                                             {"Insert", Key::Insert},
                                                             {"Delete", Key::Delete},
                                                             {"Right", Key::Right},
                                                             {"Left", Key::Left},
                                                             {"Down", Key::Down},
                                                             {"Up", Key::Up},
                                                             {"PageUp", Key::PageUp},
                                                             {"PageDown", Key::PageDown},
                                                             {"Home", Key::Home},
                                                             {"End", Key::End},
                                                             {"CapsLock", Key::CapsLock},
                                                             {"ScrollLock", Key::ScrollLock},
                                                             {"NumLock", Key::NumLock},
                                                             {"PrintScreen", Key::PrintScreen},
                                                             {"Pause", Key::Pause},
                                                             {"F1", Key::F1},
                                                             {"F2", Key::F2},
                                                             {"F3", Key::F3},
                                                             {"F4", Key::F4},
                                                             {"F5", Key::F5},
                                                             {"F6", Key::F6},
                                                             {"F7", Key::F7},
                                                             {"F8", Key::F8},
                                                             {"F9", Key::F9},
                                                             {"F10", Key::F10},
                                                             {"F11", Key::F11},
                                                             {"F12", Key::F12},
                                                             {"F13", Key::F13},
                                                             {"F14", Key::F14},
                                                             {"F15", Key::F15},
                                                             {"F16", Key::F16},
                                                             {"F17", Key::F17},
                                                             {"F18", Key::F18},
                                                             {"F19", Key::F19},
                                                             {"F20", Key::F20},
                                                             {"F21", Key::F21},
                                                             {"F22", Key::F22},
                                                             {"F23", Key::F23},
                                                             {"F24", Key::F24},
                                                             {"F25", Key::F25},
                                                             {"KP_0", Key::KP_0},
                                                             {"KP_1", Key::KP_1},
                                                             {"KP_2", Key::KP_2},
                                                             {"KP_3", Key::KP_3},
                                                             {"KP_4", Key::KP_4},
                                                             {"KP_5", Key::KP_5},
                                                             {"KP_6", Key::KP_6},
                                                             {"KP_7", Key::KP_7},
                                                             {"KP_8", Key::KP_8},
                                                             {"KP_9", Key::KP_9},
                                                             {"KP_Decimal", Key::KP_Decimal},
                                                             {"KP_Divide", Key::KP_Divide},
                                                             {"KP_Multiply", Key::KP_Multiply},
                                                             {"KP_Subtract", Key::KP_Subtract},
                                                             {"KP_Add", Key::KP_Add},
                                                             {"KP_Enter", Key::KP_Enter},
                                                             {"KP_Equal", Key::KP_Equal},
                                                             {"LeftShift", Key::LeftShift},
                                                             {"LeftControl", Key::LeftControl},
                                                             {"LeftAlt", Key::LeftAlt},
                                                             {"LeftSuper", Key::LeftSuper},
                                                             {"RightShift", Key::RightShift},
                                                             {"RightControl", Key::RightControl},
                                                             {"RightAlt", Key::RightAlt},
                                                             {"RightSuper", Key::RightSuper}};

const std::unordered_map<std::string_view, Mouse> nameToMouse = {
    {"1", Mouse::_1},      {"2", Mouse::_2},        {"3", Mouse::_3},          {"4", Mouse::_4},
    {"5", Mouse::_5},      {"6", Mouse::_6},        {"7", Mouse::_7},          {"8", Mouse::_8},
    {"Left", Mouse::Left}, {"Right", Mouse::Right}, {"Middle", Mouse::Middle},
};

} // namespace

const Window* Input::window;

bool Input::isPressed(Key key)
{
	return glfwGetKey(window->getHandle(), static_cast<int>(key)) == GLFW_PRESS;
}

bool Input::isPressed(Mouse key)
{
	return glfwGetMouseButton(window->getHandle(), static_cast<int>(key)) == GLFW_PRESS;
}

Vector2 Input::getMousePosition()
{
	Vector2d pos;
	glfwGetCursorPos(window->getHandle(), &pos.x, &pos.y);
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

Key Input::getKeyByName(std::string_view name)
{
	return nameToKey.at(name);
}

Mouse Input::getMouseByName(std::string_view name)
{
	return nameToMouse.at(name);
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
