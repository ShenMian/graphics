// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKInstance.h"

VKInstance::VKInstance(VkInstance instance)
	: handle(instance)
{
}

VKInstance::~VKInstance()
{
	vkDestroyInstance(handle, nullptr);
}

VKInstance::operator VkInstance()
{
	return handle;
}

VKInstance::operator VkInstance() const
{
	return handle;
}
