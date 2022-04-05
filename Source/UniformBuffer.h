// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>

class UniformBuffer
{
public:
	UniformBuffer(std::string_view name, int binding);

protected:
	std::string name;
	int         binding;
};
