// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Menu.h"
#include <imgui.h>

namespace ui
{

Menu::Menu(const std::string& label) : Widget(label)
{
}

void Menu::add_item(const std::string& label, const std::function<void()>& callback)
{
	items_.emplace_back(label, callback);
}

void Menu::update()
{
	if(!ImGui::BeginMenu(get_label().c_str()))
	{
		ImGui::EndMenu();
		return;
	}

	for(const auto& [label, callback] : items_)
	{
		if(ImGui::MenuItem(label.c_str()))
			callback();
	}

	ImGui::EndMenu();
}

} // namespace ui
