// Copyright 2022 ShenMian
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

	const VkExtent2D& get_size() const;

	const std::vector<VkImage>&       get_images() const;
	const std::vector<VkImageView>&   get_image_views() const;
	VkRenderPass                      get_render_pass() const;
	const std::vector<VkFramebuffer>& get_framebuffers() const;
	const std::vector<VkSemaphore>&   get_image_available_semaphores() const;
	const std::vector<VkSemaphore>&   get_render_finished_semaphores() const;

	void destroy();

	operator VkSwapchainKHR();
	operator VkSwapchainKHR() const;

private:
	void create_image_views();
	void create_render_pass();
	void create_framebuffers();
	void create_sync_objects();
	void destroy_image_views();
	void destroy_framebuffers();
	void destroy_sync_objects();

	VkSwapchainKHR handle_;
	VKDevice       device_;
	VkExtent2D     size_;
	VkFormat       image_format_;

	std::vector<VkImage>     images_;
	std::vector<VkImageView> image_views_;

	VkRenderPass               render_pass_;
	std::vector<VkFramebuffer> framebuffers_;

	std::vector<VkSemaphore> image_available_semaphores_;
	std::vector<VkSemaphore> render_finished_semaphores_;
};
