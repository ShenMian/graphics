// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "PipelineLayout.h"
#include <memory>

class Program;

/**
 * @brief 图形管线.
 */
class Pipeline
{
public:
	struct Descriptor
	{
		PipelineLayout           layout;
		std::shared_ptr<Program> program;
	};

	static std::shared_ptr<Pipeline> create(const Descriptor& desc);

	Pipeline(std::shared_ptr<Program> program);

	std::shared_ptr<Program> program;
};
