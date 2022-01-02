// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Window.h"
#include <imgui.h>

namespace ui
{

Window::Window(const std::string& label, Flag flags)
	: Widget(label), flags(flags)
{
}

void Window::add(Widget& widget)
{
	widgets.push_back(&widget);
}

void Window::update()
{
	if(ImGui::Begin(handle.c_str(), nullptr, flags))
	{
		for(auto& widget : widgets)
			widget->update();
		ImGui::End();
	}
}

} // namespace ui
