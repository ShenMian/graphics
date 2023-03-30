// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "PipelineLayout.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>

PipelineLayout::PipelineLayout(const std::initializer_list<Binding>& list)
{
	for(const auto& attr : list)
		add_binding(attr);
}

const std::vector<PipelineLayout::Binding>& PipelineLayout::get_bindings() const
{
	return bindings_;
}

void PipelineLayout::add_binding(Binding attr)
{
	assert(std::find_if(bindings_.begin(), bindings_.end(), [&](auto v) { return v.binding == attr.binding; }) ==
	       bindings_.end());
	bindings_.push_back(attr);
}
