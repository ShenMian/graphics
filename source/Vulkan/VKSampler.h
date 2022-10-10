// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Sampler.h"
#include <vulkan/vulkan.h>

class VKSampler : public Sampler
{
public:
	VKSampler(const Descriptor& desc);
	~VKSampler();

private:
	VkSampler handle;
};
