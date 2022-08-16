// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <cstdint>
#include <initializer_list>
#include <string_view>
#include <vector>

/**
 * @brief 图形管线绑定布局.
 */
class PipelineLayout
{
public:
	enum class Type
	{
		UniformBuffer,
		Sampler,
		Texture
	};

	struct StageFlags
	{
		enum
		{
			Vertex   = 1 << 0, ///< @see Shader::Stage::Vertex
			Geometry = 1 << 1, ///< @see Shader::Stage::Geometry
			Fragment = 1 << 2, ///< @see Shader::Stage::Fragment
			Compute  = 1 << 3  ///< @see Shader::Stage::Compute
		};
	};

	struct Binding
	{
		Binding(std::string_view name, uint32_t binding, Type type, uint32_t stageFlags, uint32_t arraySize = 1)
			: type(type), binding(binding), stageFlags(stageFlags), arraySize(arraySize)
		{
		}

		uint32_t binding;
		Type     type;
		uint32_t stageFlags;
		uint32_t arraySize;
	};

	PipelineLayout() = default;
	PipelineLayout(const std::initializer_list<Binding>& list);

	void addBinding(Binding attr);

	const std::vector<Binding>& getBindings() const;

private:
	std::vector<Binding> bindings;
};
