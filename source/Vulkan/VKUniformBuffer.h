// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "UniformBuffer.h"
#include "VKBuffer.h"
#include <vulkan/vulkan.h>

class VKUniformBuffer : public UniformBuffer
{
public:
	VKUniformBuffer(int binding, size_t size);

	Buffer& getBuffer() override;

private:
	VKBuffer buffer;
};
