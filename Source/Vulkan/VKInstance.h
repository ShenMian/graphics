// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <vulkan/vulkan.h>

class VKInstance final
{
public:
	VKInstance(VkInstance instance);
	~VKInstance();

	operator VkInstance() const;

private:
	VkInstance handle;
};
