// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VertexFormat.h"
#include <algorithm>
#include <cassert>
#include <unordered_map>

namespace
{

std::unordered_map<Format, uint32_t> sizeOf = {
    {Format::R16I, 2}, {Format::RG16I, 2 * 2}, {Format::RGB16I, 2 * 3}, {Format::RGBA16I, 2 * 4},
    {Format::R32I, 4}, {Format::RG32I, 4 * 2}, {Format::RGB32I, 4 * 3}, {Format::RGBA32I, 4 * 4},

    {Format::R16U, 2}, {Format::RG16U, 2 * 2}, {Format::RGB16U, 2 * 3}, {Format::RGBA16U, 2 * 4},
    {Format::R32U, 4}, {Format::RG32U, 4 * 2}, {Format::RGB32U, 4 * 3}, {Format::RGBA32U, 4 * 4},

    {Format::R16F, 2}, {Format::RG16F, 2 * 2}, {Format::RGB16F, 2 * 3}, {Format::RGBA16F, 2 * 4},
    {Format::R32F, 4}, {Format::RG32F, 4 * 2}, {Format::RGB32F, 4 * 3}, {Format::RGBA32F, 4 * 4},
};

}

uint32_t VertexFormat::Attribute::get_size() const
{
	assert(sizeOf.contains(format));
	return sizeOf[format];
}

VertexFormat::VertexFormat(const std::initializer_list<Attribute>& list)
{
	for(const auto& attr : list)
		add_attribute(attr);
}

void VertexFormat::add_attribute(Attribute attr)
{
	if(attr.location == -1)
		attr.location = static_cast<uint32_t>(attribs_.size());
	if(attr.offset == -1)
		attr.offset = stride_;
	stride_ += attr.get_size();

	assert(std::find_if(attribs_.begin(), attribs_.end(),
	                    [&](auto v) { return v.location == attr.location && v.name == attr.name; }) == attribs_.end());
	attribs_.push_back(attr);
}

const std::vector<VertexFormat::Attribute> VertexFormat::get_attributes() const
{
	return attribs_;
}

void VertexFormat::set_stride(uint32_t stride)
{
	this->stride_ = stride;
}

uint32_t VertexFormat::get_stride() const
{
	return stride_;
}
