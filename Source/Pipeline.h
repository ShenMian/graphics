// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <memory>

class Program;

class Pipeline
{
public:
	Pipeline(std::shared_ptr<Program> program);

// private:
	std::shared_ptr<Program> program;
};
