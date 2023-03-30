// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Button.h"
#include <imgui.h>

namespace ui
{

Button::Button(const std::string& label, const Vector2& size) : Widget(label), size_(size)
{
}

void Button::update()
{
	if(ImGui::Button(handle_.c_str(), ImVec2(size_.x(), size_.y())))
		if(click)
			click(*this);
	if(ImGui::IsItemHovered())
		if(hover)
			hover(*this);
}

} // namespace ui
