// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "PipelineLayout.h"
#include <memory>

class Program;
class VertexBuffer;

/**
 * @brief 图形管线.
 */
class Pipeline
{
public:
	struct Descriptor
	{
		PipelineLayout                layout;
		std::shared_ptr<VertexBuffer> vertexBuffer;
		std::shared_ptr<Program>      program;
	};

	static std::shared_ptr<Pipeline> create(const Descriptor& desc);

	Pipeline(const Descriptor& desc);

	std::shared_ptr<Program> program;
};
