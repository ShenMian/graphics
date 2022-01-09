// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VertexLayout.h"
#include <cassert>
#include <unordered_map>

namespace
{

std::unordered_map<Format, size_t> sizeOf = {
	{Format::R32F, 4},
	{Format::RG32F, 4 * 2},
	{Format::RGB32F, 4 * 3},
	{Format::RGBA32F, 4 * 4}
};

}

size_t VertexLayout::Attribute::getSize() const
{
	assert(sizeOf.contains(format));
	return sizeOf[format];
}

VertexLayout::VertexLayout(const std::initializer_list<Attribute>& list)
{
	for(const auto& attr : list)
		addAttribute(attr);
}

void VertexLayout::addAttribute(Attribute attr)
{
	assert(std::find_if(attribs.begin(), attribs.end(), [&](auto v) { return v.name == attr.name; }) == attribs.end());

	attr.offset = stride;
	stride += attr.getSize();
	attribs.push_back(attr);
}

const std::vector<VertexLayout::Attribute> VertexLayout::getAttributes() const
{
	return attribs;
}

void VertexLayout::setStride(size_t stride)
{
	this->stride = stride;
}

size_t VertexLayout::getStride() const
{
	return stride;
}
