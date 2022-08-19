// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Button.h"
#include <imgui.h>

namespace ui
{

Button::Button(const std::string& label, const Vector2& size) : Widget(label), size(size)
{
}

void Button::update()
{
	if(ImGui::Button(handle.c_str(), ImVec2(size.x, size.y)))
		if(click)
			click(*this);
	if(ImGui::IsItemHovered())
		if(hover)
			hover(*this);
}

} // namespace ui
