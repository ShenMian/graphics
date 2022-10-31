// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "SwapchainBuilder.h"
#include <algorithm>
#include <stdexcept>

SwapchainBuilder::SwapchainBuilder(VKDevice& device) : device(device)
{
}

VKSwapchain SwapchainBuilder::build()
{
	const auto& physicalDevice = device.getPhysicalDevice();
	const auto  capabilities   = physicalDevice.getSurfaceCapabilities();

	const auto format = getSurfaceFormat();
	const auto extent = getExtent();

	swapchainInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface          = physicalDevice.getSurface();
	swapchainInfo.minImageCount    = getImageCount();
	swapchainInfo.imageFormat      = format.format;
	swapchainInfo.imageColorSpace  = format.colorSpace;
	swapchainInfo.imageExtent      = extent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const uint32_t queueFamilyIndices[] = {physicalDevice.graphics.value(), physicalDevice.present.value()};
	if(physicalDevice.graphics != physicalDevice.present)
	{
		swapchainInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.queueFamilyIndexCount = 2;
		swapchainInfo.pQueueFamilyIndices   = queueFamilyIndices;
	}
	else
	{
		swapchainInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices   = nullptr;
	}

	swapchainInfo.preTransform   = capabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode    = getPresentMode();
	swapchainInfo.clipped        = true;
	swapchainInfo.oldSwapchain   = VK_NULL_HANDLE;

	VkSwapchainKHR swapchain;
	if(vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain) != VK_SUCCESS)
		throw std::runtime_error("failed to create swapchain");

	return {swapchain, device, format.format, extent};
}

SwapchainBuilder& SwapchainBuilder::setDesiredFormat(VkSurfaceFormatKHR format) noexcept
{
	info.desiredFormat = format;
	return *this;
}

SwapchainBuilder& SwapchainBuilder::setDesiredPresentMode(VkPresentModeKHR presentMode) noexcept
{
	info.desiredPresentMode = presentMode;
	return *this;
}

SwapchainBuilder& SwapchainBuilder::setDesiredExtent(VkExtent2D extent) noexcept
{
	info.desiredExtent = extent;
	return *this;
}

uint32_t SwapchainBuilder::getImageCount() const
{
	const auto capabilities = device.getPhysicalDevice().getSurfaceCapabilities();
	const auto imageCount   = capabilities.minImageCount + 1;
	if(capabilities.maxImageCount != 0 && imageCount > capabilities.maxImageCount)
		return capabilities.maxImageCount;
	return imageCount;
}

VkSurfaceFormatKHR SwapchainBuilder::getSurfaceFormat() const
{
	const auto         formats = device.getPhysicalDevice().getSurfaceFormats();
	VkSurfaceFormatKHR format  = formats[0];
	for(const auto& fmt : formats)
		if(fmt.format == info.desiredFormat.format && fmt.colorSpace == info.desiredFormat.colorSpace)
			format = fmt;
	return format;
}

VkPresentModeKHR SwapchainBuilder::getPresentMode() const
{
	const auto       presentModes = device.getPhysicalDevice().getSurfacePresentModes();
	VkPresentModeKHR presentMode  = VK_PRESENT_MODE_FIFO_KHR;
	for(auto mode : presentModes)
		if(mode == info.desiredPresentMode)
			presentMode = mode;
	return presentMode;
}

VkExtent2D SwapchainBuilder::getExtent() const
{
	const auto capabilities = device.getPhysicalDevice().getSurfaceCapabilities();
	auto       extent       = capabilities.currentExtent;
	if(capabilities.currentExtent.width == -1)
	{
		extent.width =
		    std::clamp(info.desiredExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(info.desiredExtent.height, capabilities.minImageExtent.height,
		                           capabilities.maxImageExtent.height);
	}
	return extent;
}
