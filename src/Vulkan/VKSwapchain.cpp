// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKSwapchain.h"
#include <stdexcept>

VKSwapchain::VKSwapchain(VkSwapchainKHR swapchain, VKDevice& device, VkFormat imageFormat, VkExtent2D extent)
    : handle(swapchain), device(device), imageFormat(imageFormat), size(extent)
{
	uint32_t imageCount;
	if(vkGetSwapchainImagesKHR(device, handle, &imageCount, nullptr) != VK_SUCCESS)
		throw std::runtime_error("failed to get swapchain images");
	images.resize(imageCount);
	vkGetSwapchainImagesKHR(device, handle, &imageCount, images.data());

	createImageViews();
	createRenderPass();
	createFramebuffers();
	createSyncObjects();
}

const std::vector<VkImage>& VKSwapchain::getImages() const
{
	return images;
}

const std::vector<VkImageView>& VKSwapchain::getImageViews() const
{
	return imageViews;
}

VkRenderPass VKSwapchain::getRenderPass() const
{
	return renderPass;
}

const std::vector<VkFramebuffer>& VKSwapchain::getFramebuffers() const
{
	return framebuffers;
}

const std::vector<VkSemaphore>& VKSwapchain::getImageAvailableSemaphores() const
{
	return imageAvailableSemaphores;
}

const std::vector<VkSemaphore>& VKSwapchain::getRenderFinishedSemaphores() const
{
	return renderFinishedSemaphores;
}

void VKSwapchain::destroy()
{
	destroySyncObjects();
	destroyFramebuffers();
	vkDestroyRenderPass(device, renderPass, nullptr);
	destroyImageViews();
	vkDestroySwapchainKHR(device, handle, nullptr);
}

const VkExtent2D& VKSwapchain::getSize() const
{
	return size;
}

VKSwapchain::operator VkSwapchainKHR()
{
	return handle;
}

VKSwapchain::operator VkSwapchainKHR() const
{
	return handle;
}

void VKSwapchain::createImageViews()
{
	imageViews.resize(images.size());
	for(size_t i = 0; i < images.size(); i++)
	{
		VkImageViewCreateInfo imageViewInfo           = {};
		imageViewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image                           = images[i];
		imageViewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format                          = imageFormat;
		imageViewInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel   = 0;
		imageViewInfo.subresourceRange.levelCount     = 1;
		imageViewInfo.subresourceRange.baseArrayLayer = 0;
		imageViewInfo.subresourceRange.layerCount     = 1;

		if(vkCreateImageView(device, &imageViewInfo, nullptr, &imageViews[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create swapchain image view");
	}
}

void VKSwapchain::createRenderPass()
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

	if(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		throw std::runtime_error("failed to create render pass");
}

void VKSwapchain::createFramebuffers()
{
	framebuffers.resize(imageViews.size());
	for(size_t i = 0; i < imageViews.size(); i++)
	{
		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass              = renderPass;
		framebufferInfo.attachmentCount         = 1;
		framebufferInfo.pAttachments            = &imageViews[i];
		framebufferInfo.width                   = size.width;
		framebufferInfo.height                  = size.height;
		framebufferInfo.layers                  = 1;

		if(vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create framebuffer");
	}
}

void VKSwapchain::createSyncObjects()
{
	constexpr int maxFramesInFlight = 2;

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType                 = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags             = VK_FENCE_CREATE_SIGNALED_BIT;

	imageAvailableSemaphores.resize(maxFramesInFlight);
	renderFinishedSemaphores.resize(maxFramesInFlight);
	for(size_t i = 0; i < maxFramesInFlight; i++)
	{
		if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphore");
		if(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
			throw std::runtime_error("failed to create semaphore");
	}
}

void VKSwapchain::destroyImageViews()
{
	for(auto view : imageViews)
		vkDestroyImageView(device, view, nullptr);
}

void VKSwapchain::destroyFramebuffers()
{
	for(auto framebuffer : framebuffers)
		vkDestroyFramebuffer(device, framebuffer, nullptr);
}

void VKSwapchain::destroySyncObjects()
{
	for(size_t i = 0; i < imageAvailableSemaphores.size(); i++)
		vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
	for(size_t i = 0; i < renderFinishedSemaphores.size(); i++)
		vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
}
