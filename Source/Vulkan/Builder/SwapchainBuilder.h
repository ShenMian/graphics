// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKDevice.h"
#include "../VKSwapchain.h"

class SwapchainBuilder
{
public:
	SwapchainBuilder(VKDevice& device);

	VKSwapchain build();

	SwapchainBuilder& setDesiredFormat(VkSurfaceFormatKHR format);

	SwapchainBuilder& setDesiredPresentMode(VkPresentModeKHR presentMode);

	SwapchainBuilder& setDesiredExtent(VkExtent2D extent);

private:
	VkSwapchainCreateInfoKHR swapchainInfo = {};

	VKDevice& device;

	struct
	{
		VkSurfaceFormatKHR desiredFormat = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		VkPresentModeKHR desiredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		VkExtent2D desiredExtent = {};
	} info;
};
