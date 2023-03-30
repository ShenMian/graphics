// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <vulkan/vulkan.h>

/**
 * @brief Vulkan 实例.
 */
class VKInstance final
{
public:
	VKInstance() = default;
	VKInstance(VkInstance instance);

	void destroy();

	operator VkInstance() noexcept;
	operator VkInstance() const noexcept;

private:
	VkInstance handle_;
};
