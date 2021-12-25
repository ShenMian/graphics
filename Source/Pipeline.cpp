// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Pipeline.h"

std::shared_ptr<Pipeline> Pipeline::create(std::shared_ptr<Program> program)
{
	return std::make_shared<Pipeline>(program);
}

Pipeline::Pipeline(std::shared_ptr<Program> program)
	: program(program)
{
}
