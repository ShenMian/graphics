// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKSwapchain.h"
#include <stdexcept>

VKSwapchain::VKSwapchain(VkSwapchainKHR swapchain, VKDevice& device, VkFormat imageFormat, VkExtent2D extent)
	: handle(swapchain), device(device), imageFormat(imageFormat), extent(extent)
{
	uint32_t imageCount;
	if(vkGetSwapchainImagesKHR(device, handle, &imageCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error("failed to get swapchain images");
	images.resize(imageCount);
	vkGetSwapchainImagesKHR(device, handle, &imageCount, images.data());

	imageViews.resize(images.size());
	for(size_t i = 0; i < images.size(); i++)
	{
		VkImageViewCreateInfo imageViewInfo = {};
		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image = images[i];
		imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format = imageFormat;
		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel = 0;
		imageViewInfo.subresourceRange.levelCount = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount = 1;

		if(vkCreateImageView(device, &imageViewInfo, nullptr, &imageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create swapchain image view");
	}
}

VKSwapchain::~VKSwapchain()
{
	/*
	for(auto view : imageViews) {
		vkDestroyImageView(device, view, nullptr);
	}
	vkDestroySwapchainKHR(device, handle, nullptr);
	*/
}

const std::vector<VkImage>& VKSwapchain::getImages() const
{
	return images;
}

const std::vector<VkImageView>& VKSwapchain::getImageViews() const
{
	return imageViews;
}

const VkExtent2D& VKSwapchain::getExtent() const
{
	return extent;
}

VKSwapchain::operator VkSwapchainKHR()
{
	return handle;
}

VKSwapchain::operator VkSwapchainKHR() const
{
	return handle;
}
