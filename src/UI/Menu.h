// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <math/math.hpp>
#include <utility>
#include <vector>

namespace ui
{

class Menu : public Widget
{
public:
	Menu(const std::string& label);

	void add_item(const std::string& label, const std::function<void()>& callback);

	void update() override;

private:
	std::vector<std::pair<std::string, std::function<void()>>> items_;
};

} // namespace ui
