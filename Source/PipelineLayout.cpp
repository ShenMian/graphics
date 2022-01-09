// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "PipelineLayout.h"

PipelineLayout::PipelineLayout(const std::initializer_list<Attribute>& list)
	: attribs(list)
{
}

const std::vector<PipelineLayout::Attribute> PipelineLayout::getAttributes() const
{
	return attribs;
}
