// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VertexFormat.h"
#include <unordered_map>
#include <cassert>

namespace
{

std::unordered_map<Format, size_t> sizeOf = {
	{Format::R32F, 4},
	{Format::RG32F, 4 * 2},
	{Format::RGB32F, 4 * 3},
	{Format::RGBA32F, 4 * 4}
};

}

size_t VertexFormat::Attribute::getSize() const
{
	assert(sizeOf.contains(format));
	return sizeOf[format];
}

VertexFormat::VertexFormat(const std::initializer_list<Attribute>& list)
	: attribs(list)
{
	for(auto& attr : attribs)
	{
		attr.offset = stride;
		stride += attr.getSize();
	}
}

const std::vector<VertexFormat::Attribute> VertexFormat::getAttributes() const
{
	return attribs;
}

void VertexFormat::setStride(size_t stride)
{
	this->stride = stride;
}

size_t VertexFormat::getStride() const
{
	return stride;
}