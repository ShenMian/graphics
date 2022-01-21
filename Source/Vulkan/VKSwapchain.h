// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKDevice.h"
#include <vulkan/vulkan.h>
#include <vector>

class VKSwapchain
{
public:
	VKSwapchain() = default;
	VKSwapchain(VkSwapchainKHR swapchain, VKDevice& device, VkFormat imageFormat, VkExtent2D extent);

	operator VkSwapchainKHR();
	operator VkSwapchainKHR() const;

private:
	VkSwapchainKHR handle;
	VKDevice       device;
	VkFormat       imageFormat;
	VkExtent2D     extent;

	std::vector<VkImage>     images;
	std::vector<VkImageView> imageViews;
};
