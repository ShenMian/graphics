// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Window.h"
#include <imgui.h>

namespace ui
{

Window::Window(const std::string& label, Flag flags) : Widget(label), flags_(flags)
{
}

void Window::add(Widget& widget)
{
	widgets_.push_back(&widget);
}

void Window::clear()
{
	widgets_.clear();
}

void Window::update()
{
	if(!ImGui::Begin(handle_.c_str(), nullptr, flags_))
	{
		ImGui::End();
		return;
	}

	for(auto& widget : widgets_)
		widget->update();
	if(ImGui::IsWindowHovered())
		if(hover)
			hover(*this);

	ImGui::End();
}

} // namespace ui
