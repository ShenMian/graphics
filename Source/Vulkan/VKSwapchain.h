// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKDevice.h"
#include <vulkan/vulkan.h>
#include <vector>

class VKSwapchain final
{
public:
	VKSwapchain() = default;
	VKSwapchain(VkSwapchainKHR swapchain, VKDevice& device, VkFormat imageFormat, VkExtent2D extent);
	~VKSwapchain();

	const std::vector<VkImage>& getImages() const;
	const std::vector<VkImageView>& getImageViews() const;

	const VkExtent2D& getExtent() const;

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
