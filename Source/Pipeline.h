// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <memory>

class Program;

class Pipeline
{
public:
	static std::shared_ptr<Pipeline> create(std::shared_ptr<Program> program);

	Pipeline(std::shared_ptr<Program> program);

// private:
	std::shared_ptr<Program> program;
};
