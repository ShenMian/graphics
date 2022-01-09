// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKBuffer.h"
#include <vulkan/vulkan.h>

class VKUniformBuffer
{
public:
	VKUniformBuffer(size_t size);

	void write(const void* data, size_t size, size_t offset);

private:
	VKBuffer buffer;
};
