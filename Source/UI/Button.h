// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <Math/Math.hpp>

namespace ui
{

class Button : public Widget
{
public:
	Button(const std::string& label, const Vector2& size = Vector2::zero);

	void update() override;

	std::function<void(Button&)> on;

private:
	Vector2 size;
};

} // namespace ui
