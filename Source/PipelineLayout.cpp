// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "PipelineLayout.h"
#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>

PipelineLayout::PipelineLayout(const std::initializer_list<Binding>& list)
{
	for(const auto& attr : list)
		addBinding(attr);
}

const std::vector<PipelineLayout::Binding>& PipelineLayout::getBindings() const
{
	return bindings;
}

void PipelineLayout::addBinding(Binding attr)
{
	assert(std::find_if(bindings.begin(), bindings.end(), [&](auto v) { return v.slot == attr.slot; }) ==
	       bindings.end());
	bindings.push_back(attr);
}
