// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Format.h"
#include <initializer_list>
#include <string_view>
#include <vector>

/**
 * @brief 顶点格式布局.
 */
class VertexFormat
{
public:
	struct Attribute
	{
		Attribute(std::string_view name, Format fmt, uint32_t offset = -1, bool normalized = false)
		    : name(name), format(fmt), offset(offset), normalized(normalized)
		{
		}

		Attribute(uint32_t location, std::string_view name, Format fmt, bool normalized = false)
		    : location(location), name(name), format(fmt), normalized(normalized)
		{
		}

		uint32_t get_size() const;

		uint32_t         location = -1;
		std::string_view name;
		Format           format;
		bool             normalized = false;
		uint32_t         offset     = -1;
	};

	VertexFormat() = default;
	VertexFormat(const std::initializer_list<Attribute>& list);

	void add_attribute(Attribute attr);

	const std::vector<Attribute> get_attributes() const;

	void     set_stride(uint32_t stride);
	uint32_t get_stride() const;

private:
	std::vector<Attribute> attribs_;
	uint32_t               stride_ = 0;
};
