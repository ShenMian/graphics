// Copyright 2021 ShenMian
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
		Attribute(std::string_view name, Format fmt, bool normalized = false)
		    : location(-1), name(name), format(fmt), normalized(normalized)
		{
		}

		Attribute(uint32_t location, std::string_view name, Format fmt, bool normalized = false)
		    : location(location), name(name), format(fmt), normalized(normalized)
		{
		}

		uint32_t getSize() const;

		uint32_t         location;
		std::string_view name;
		Format           format;
		bool             normalized = false;
		uint32_t         offset     = 0;
	};

	VertexFormat() = default;
	VertexFormat(const std::initializer_list<Attribute>& list);

	void addAttribute(Attribute attr);

	const std::vector<Attribute> getAttributes() const;

	void     setStride(uint32_t stride);
	uint32_t getStride() const;

private:
	std::vector<Attribute> attribs;
	uint32_t               stride = 0;
};
