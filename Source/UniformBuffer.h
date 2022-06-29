// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>

class Buffer;

class UniformBuffer
{
public:
	UniformBuffer(int binding);
	
	virtual Buffer& getBuffer() = 0;

protected:
	int binding;
};
