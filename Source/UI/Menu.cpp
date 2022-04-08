// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Menu.h"
#include <imgui.h>

namespace ui
{

Menu::Menu(const std::string& label)
	: Widget(label)
{
}

void Menu::addItem(const std::string& label, const std::function<void()>& callback)
{
	items.emplace_back(label, callback);
}

void Menu::update()
{
	if(!ImGui::BeginMenu(getLabel().c_str()))
	{
		ImGui::EndMenu();
		return;
	}

	for(const auto& [label, callback] : items)
	{
		if(ImGui::MenuItem(label.c_str()))
			callback();
	}

	ImGui::EndMenu();
}

} // namespace ui
