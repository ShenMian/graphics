// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <Math/Math.hpp>

namespace ui
{

class Label : public Widget
{
public:
	Label(const std::string& label);

	void setText(const std::string& text);

	void update() override;

private:
	std::string text;
	Vector3f    color = {1, 1, 1};
};

} // namespace ui
