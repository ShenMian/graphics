// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>

class UniformBuffer
{
public:
	UniformBuffer(const std::string& name, int slot);

protected:
	std::string name;
	int         slot;
};
