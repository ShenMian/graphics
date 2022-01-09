// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <cstdint>
#include <initializer_list>
#include <vector>

/**
 * @brief 图形管线绑定布局.
 */
class PipelineLayout
{
public:
	enum class Type
	{
		Sampler,
		Texture
	};

	struct StageFlags
	{
		enum
		{
			Vertex,
			Geometry,
			Fragment,
			Pixel = Fragment,
			Compute
		};
	};

	struct Attribute
	{
		Attribute(Type type, uint32_t slot, int stageFlags, uint32_t arraySize = 1)
			: type(type), slot(slot), stageFlags(stageFlags), arraySize(arraySize)
		{
		}

		Type       type;
		uint32_t   slot;
		int        stageFlags;
		uint32_t   arraySize;
	};

	PipelineLayout(const std::initializer_list<Attribute>& list);

	const std::vector<Attribute> getAttributes() const;

private:
	std::vector<Attribute> attribs;
};
