// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Widget.h"
#include <math/math.hpp>

namespace ui
{

/**
 * @brief 文本标签.
 */
class Label : public Widget
{
public:
	Label(const std::string& label = "");

	void setText(const std::string& text);

	void update() override;

private:
	std::string text;
	Vector3f    color = {1, 1, 1};
};

} // namespace ui
