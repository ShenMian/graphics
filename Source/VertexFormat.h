// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Format.h"
#include <initializer_list>
#include <vector>

/**
 * @brief 顶点格式.
 */
class VertexFormat
{
public:
	struct Attribute
	{
		Attribute(const char* name, Format fmt, bool normalized = false)
			: name(name), format(fmt), normalized(normalized)
		{
		}

		size_t getSize() const;

		const char* name;
		Format      format;
		bool        normalized = false;
		size_t      offset = 0;
	};

	VertexFormat(const std::initializer_list<Attribute>& list);

	const std::vector<Attribute> getAttributes() const;

	void   setStride(size_t stride);
	size_t getStride() const;

private:
	std::vector<Attribute> attribs;
	size_t                 stride = 0;
};
