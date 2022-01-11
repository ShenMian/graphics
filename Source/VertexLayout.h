// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Format.h"
#include <string_view>
#include <initializer_list>
#include <vector>

/**
 * @brief 顶点格式布局.
 */
class VertexLayout
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

		size_t getSize() const;

		uint32_t         location;
		std::string_view name;
		Format           format;
		bool             normalized = false;
		uint32_t         offset = 0;
	};

	VertexLayout(const std::initializer_list<Attribute>& list);

	void addAttribute(Attribute attr);

	const std::vector<Attribute> getAttributes() const;

	void   setStride(size_t stride);
	size_t getStride() const;

private:
	std::vector<Attribute> attribs;
	size_t                 stride = 0;
};
