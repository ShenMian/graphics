// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKSwapchain.h"
#include <stdexcept>

VKSwapchain::VKSwapchain(VkSwapchainKHR swapchain, VKDevice& device, VkFormat imageFormat, VkExtent2D extent)
    : handle_(swapchain), device_(device), image_format_(imageFormat), size_(extent)
{
	uint32_t imageCount;
	if(vkGetSwapchainImagesKHR(device, handle_, &imageCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error("failed to get swapchain images");
	images_.resize(imageCount);
	vkGetSwapchainImagesKHR(device, handle_, &imageCount, images_.data());

	create_image_views();
	create_render_pass();
	create_framebuffers();
	create_sync_objects();
}

const std::vector<VkImage>& VKSwapchain::get_images() const
{
	return images_;
}

const std::vector<VkImageView>& VKSwapchain::get_image_views() const
{
	return image_views_;
}

VkRenderPass VKSwapchain::get_render_pass() const
{
	return render_pass_;
}

const std::vector<VkFramebuffer>& VKSwapchain::get_framebuffers() const
{
	return framebuffers_;
}

const std::vector<VkSemaphore>& VKSwapchain::get_image_available_semaphores() const
{
	return image_available_semaphores_;
}

const std::vector<VkSemaphore>& VKSwapchain::get_render_finished_semaphores() const
{
	return render_finished_semaphores_;
}

void VKSwapchain::destroy()
{
	destroy_sync_objects();
	destroy_framebuffers();
	vkDestroyRenderPass(device_, render_pass_, nullptr);
	destroy_image_views();
	vkDestroySwapchainKHR(device_, handle_, nullptr);
}

const VkExtent2D& VKSwapchain::get_size() const
{
	return size_;
}

VKSwapchain::operator VkSwapchainKHR()
{
	return handle_;
}

VKSwapchain::operator VkSwapchainKHR() const
{
	return handle_;
}

void VKSwapchain::create_image_views()
{
	image_views_.resize(images_.size());
	for(size_t i = 0; i < images_.size(); i++)
	{
		VkImageViewCreateInfo imageViewInfo           = {};
		imageViewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image                           = images_[i];
		imageViewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format                          = image_format_;
		imageViewInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel   = 0;
		imageViewInfo.subresourceRange.levelCount     = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount     = 1;

		if(vkCreateImageView(device_, &imageViewInfo, nullptr, &image_views_[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create swapchain image view");
	}
}

void VKSwapchain::create_render_pass()
{
	std::vector<VkAttachmentDescription> attachments;

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format                  = VK_FORMAT_B8G8R8A8_SRGB;
	colorAttachment.samples                 = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp                  = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp                 = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp           = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp          = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout           = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout             = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachments.push_back(colorAttachment);

	// VkAttachmentDescription depthAttachment = {};
	// attachments.push_back(depthAttachment);

	std::vector<VkAttachmentReference> colorAttachmentRefs;
	VkAttachmentReference              colorAttachmentRef = {};
	colorAttachmentRef.attachment                         = 0;
	colorAttachmentRef.layout                             = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	colorAttachmentRefs.push_back(colorAttachmentRef);

	std::vector<VkSubpassDescription> subpasses;
	VkSubpassDescription              subpass = {};
	subpass.pipelineBindPoint                 = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount              = static_cast<uint32_t>(colorAttachmentRefs.size());
	subpass.pColorAttachments                 = &colorAttachmentRef;
	subpasses.push_back(subpass);

	std::vector<VkSubpassDependency> dependencies;
	VkSubpassDependency              dependency = {};
	dependency.dstSubpass                       = 0;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	dependency.dstStageMask =
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
	dependency.srcAccessMask = 0;
	dependency.srcStageMask =
	    VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	dependencies.push_back(dependency);

	VkRenderPassCreateInfo renderPassInfo = {};
	renderPassInfo.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassInfo.attachmentCount        = static_cast<uint32_t>(attachments.size());
	renderPassInfo.pAttachments           = attachments.data();
	renderPassInfo.subpassCount           = static_cast<uint32_t>(subpasses.size());
	renderPassInfo.pSubpasses             = subpasses.data();
	renderPassInfo.dependencyCount        = static_cast<uint32_t>(dependencies.size());
	renderPassInfo.pDependencies          = dependencies.data();

	if(vkCreateRenderPass(device_, &renderPassInfo, nullptr, &render_pass_) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass");
}

void VKSwapchain::create_framebuffers()
{
	framebuffers_.resize(image_views_.size());
	for(size_t i = 0; i < image_views_.size(); i++)
	{
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass              = render_pass_;
		framebufferInfo.attachmentCount         = 1;
		framebufferInfo.pAttachments            = &image_views_[i];
		framebufferInfo.width                   = size_.width;
		framebufferInfo.height                  = size_.height;
		framebufferInfo.layers                  = 1;

		if(vkCreateFramebuffer(device_, &framebufferInfo, nullptr, &framebuffers_[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer");
	}
}

void VKSwapchain::create_sync_objects()
{
	constexpr int maxFramesInFlight = 2;

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

	image_available_semaphores_.resize(maxFramesInFlight);
	render_finished_semaphores_.resize(maxFramesInFlight);
	for(size_t i = 0; i < maxFramesInFlight; i++)
	{
		if(vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &image_available_semaphores_[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphore");
		if(vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &render_finished_semaphores_[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphore");
	}
}

void VKSwapchain::destroy_image_views()
{
	for(auto view : image_views_)
		vkDestroyImageView(device_, view, nullptr);
}

void VKSwapchain::destroy_framebuffers()
{
	for(auto framebuffer : framebuffers_)
		vkDestroyFramebuffer(device_, framebuffer, nullptr);
}

void VKSwapchain::destroy_sync_objects()
{
	for(size_t i = 0; i < image_available_semaphores_.size(); i++)
		vkDestroySemaphore(device_, image_available_semaphores_[i], nullptr);
	for(size_t i = 0; i < render_finished_semaphores_.size(); i++)
		vkDestroySemaphore(device_, render_finished_semaphores_[i], nullptr);
}
