// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Pipeline.h"

std::shared_ptr<Pipeline> Pipeline::create(const Descriptor& desc)
{
	return std::make_shared<Pipeline>(desc.program);
}

Pipeline::Pipeline(std::shared_ptr<Program> program)
	: program(program)
{
}
