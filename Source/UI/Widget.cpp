// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Widget.h"
#include <stdexcept>

namespace ui
{

uint64_t Widget::nextId = 0;

Widget::Widget(const std::string& label)
	: label(label), id(std::to_string(nextId++))
{
	updateHandle();
}

Widget::Widget(const std::string& label, const std::string& id)
	: label(label)
{
	updateHandle();
}

void Widget::setLabel(const std::string& label)
{
	this->label = label;
	updateHandle();
}

const std::string& Widget::getLabel() const
{
	return label;
}

void Widget::updateHandle()
{
	if(label.contains("##"))
		throw std::runtime_error("widget label cannot contain \"##\".");
	handle = label + "###" + id;
}

} // namespace ui
