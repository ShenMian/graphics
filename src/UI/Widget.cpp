// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Widget.h"
#include <stdexcept>

namespace ui
{

uint64_t Widget::next_id_ = 0;

Widget::Widget() : id_(request_id())
{
}

Widget::Widget(const std::string& label) : label_(label), id_(request_id())
{
	update_handle();
}

Widget::Widget(const std::string& label, const std::string& id) : label_(label), id_(std::stoull(id))
{
	update_handle();
}

Widget::Widget(const Widget& rhs) : label_(rhs.label_), id_(request_id())
{
	update_handle();
}

void Widget::set_label(const std::string& label)
{
	this->label_ = label;
	update_handle();
}

const std::string& Widget::get_label() const noexcept
{
	return label_;
}

uint64_t Widget::get_id() const noexcept
{
	return id_;
}

void Widget::update()
{
}

void Widget::update_handle()
{
	// if(label.contains("##"))
	if(label_.find("##") != std::string::npos)
		throw std::runtime_error("widget label cannot contain \"##\".");
	handle_ = label_ + "###" + std::to_string(id_);
}

uint64_t Widget::request_id()
{
	return next_id_++;
}

} // namespace ui
