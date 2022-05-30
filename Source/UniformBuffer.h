// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>

class Buffer;

class UniformBuffer
{
public:
	UniformBuffer(std::string_view name, int binding);
	
	virtual Buffer& getBuffer() = 0;

protected:
	std::string name;
	int         binding;
};
