// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <string>
#include <memory>

class Buffer;

class UniformBuffer
{
public:
	[[nodiscard]] static std::shared_ptr<UniformBuffer> create(int binding, size_t size);
	
	virtual Buffer& getBuffer() = 0;

protected:
	UniformBuffer(int binding);

	int binding;
};
