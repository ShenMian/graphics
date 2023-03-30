// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKInstance.h"

VKInstance::VKInstance(VkInstance instance) : handle_(instance)
{
}

void VKInstance::destroy()
{
	vkDestroyInstance(handle_, nullptr);
}

VKInstance::operator VkInstance() noexcept
{
	return handle_;
}

VKInstance::operator VkInstance() const noexcept
{
	return handle_;
}
