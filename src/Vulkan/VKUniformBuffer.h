// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "UniformBuffer.h"
#include "VKBuffer.h"
#include <vulkan/vulkan.h>

class VKUniformBuffer : public UniformBuffer
{
public:
	VKUniformBuffer(int binding, size_t size);

	Buffer& get_buffer() override;

private:
	VKBuffer buffer_;
};
