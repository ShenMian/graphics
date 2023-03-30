// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Label.h"
#include <imgui.h>

namespace ui
{

Label::Label(const std::string& label) : Widget(label), text_(label)
{
}

void Label::set_text(const std::string& text)
{
	this->text_ = text;
}

void Label::update()
{
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(color_.x(), color_.y(), color_.z(), 1.f));
	ImGui::TextUnformatted(text_.c_str());
	ImGui::PopStyleColor(1);
}

} // namespace ui
