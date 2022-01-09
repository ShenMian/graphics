// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "PipelineLayout.h"
#include <cassert>
#include <string>
#include <stdexcept>

PipelineLayout::PipelineLayout(const std::initializer_list<Attribute>& list)
{
	for(const auto& attr : list)
		addAttribute(attr);
}

const std::vector<PipelineLayout::Attribute> PipelineLayout::getAttributes() const
{
	return attribs;
}

void PipelineLayout::addAttribute(Attribute attr)
{
	assert(std::find_if(attribs.begin(), attribs.end(), [&](auto v) { return v.slot == attr.slot; }) == attribs.end());
	attribs.push_back(attr);
}
