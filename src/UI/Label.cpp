// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Label.h"
#include <imgui.h>

namespace ui
{

Label::Label(const std::string& label) : Widget(label), text(label)
{
}

void Label::setText(const std::string& text)
{
	this->text = text;
}

void Label::update()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color.r, color.g, color.b, 1.f));
	ImGui::TextUnformatted(text.c_str());
	ImGui::PopStyleColor(1);
}

} // namespace ui
