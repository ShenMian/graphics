// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Widget.h"
#include <stdexcept>

namespace ui
{

uint64_t Widget::nextId = 0;

Widget::Widget()
	: id(requestId())
{
}

Widget::Widget(const std::string& label)
	: label(label), id(requestId())
{
	updateHandle();
}

Widget::Widget(const std::string& label, const std::string& id)
	: label(label), id(std::stoull(id))
{
	updateHandle();
}

Widget::Widget(const Widget& rhs)
	: label(rhs.label), id(requestId())
{
	updateHandle();
}

void Widget::setLabel(const std::string& label)
{
	this->label = label;
	updateHandle();
}

const std::string& Widget::getLabel() const noexcept
{
	return label;
}

uint64_t Widget::getId() const noexcept
{
	return id;
}

void Widget::update()
{
}

void Widget::updateHandle()
{
	// if(label.contains("##"))
	if(label.find("##") != std::string::npos)
		throw std::runtime_error("widget label cannot contain \"##\".");
	handle = label + "###" + std::to_string(id);
}

uint64_t Widget::requestId()
{
	return nextId++;
}

} // namespace ui
