// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Window.h"
#include <imgui.h>

namespace ui
{

Window::Window(const std::string& label, Flag flags) : Widget(label), flags(flags)
{
}

void Window::add(Widget& widget)
{
	widgets.push_back(&widget);
}

void Window::clear()
{
	widgets.clear();
}

void Window::update()
{
	if(!ImGui::Begin(handle.c_str(), nullptr, flags))
	{
		ImGui::End();
		return;
	}

	for(auto& widget : widgets)
		widget->update();
	if(ImGui::IsWindowHovered())
		if(hover)
			hover(*this);

	ImGui::End();
}

} // namespace ui
