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

	void set_text(const std::string& text);

	void update() override;

private:
	std::string text_;
	Vector3f    color_ = {1, 1, 1};
};

} // namespace ui
