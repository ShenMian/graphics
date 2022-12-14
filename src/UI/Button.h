// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <math/math.hpp>

namespace ui
{

/**
 * @brief 按钮.
 */
class Button : public Widget
{
public:
	Button(const std::string& label, const Vector2& size = Vector2::zero);

	void update() override;

	std::function<void(Button&)> click;
	std::function<void(Button&)> hover;

private:
	Vector2 size;
};

} // namespace ui
