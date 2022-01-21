// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKSwapchain.h"

Swapchain::Swapchain(VkSurfaceKHR surface)
{
	VkSwapchainCreateInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	info.surface = surface;

}
