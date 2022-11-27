// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKDevice.h"
#include <vector>
#include <vulkan/vulkan.h>

class VKSwapchain final
{
public:
	VKSwapchain() = default;
	VKSwapchain(VkSwapchainKHR swapchain, VKDevice& device, VkFormat imageFormat, VkExtent2D extent);

	const VkExtent2D& getSize() const;

	const std::vector<VkImage>&       getImages() const;
	const std::vector<VkImageView>&   getImageViews() const;
	VkRenderPass                      getRenderPass() const;
	const std::vector<VkFramebuffer>& getFramebuffers() const;
	const std::vector<VkSemaphore>&   getImageAvailableSemaphores() const;
	const std::vector<VkSemaphore>&   getRenderFinishedSemaphores() const;

	void destroy();

	operator VkSwapchainKHR();
	operator VkSwapchainKHR() const;

private:
	void createImageViews();
	void createRenderPass();
	void createFramebuffers();
	void createSyncObjects();
	void destroyImageViews();
	void destroyFramebuffers();
	void destroySyncObjects();

	VkSwapchainKHR handle;
	VKDevice       device;
	VkExtent2D     size;
	VkFormat       imageFormat;

	std::vector<VkImage>     images;
	std::vector<VkImageView> imageViews;

	VkRenderPass               renderPass;
	std::vector<VkFramebuffer> framebuffers;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
};
