// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "SwapchainBuilder.h"
#include <algorithm>
#include <stdexcept>

SwapchainBuilder::SwapchainBuilder(VKDevice& device) : device_(device)
{
}

VKSwapchain SwapchainBuilder::build()
{
	const auto& physicalDevice = device_.get_physical_device();
	const auto  capabilities   = physicalDevice.get_surface_capabilities();

	const auto format = get_surface_format();
	const auto extent = get_extent();

	swapchain_info_.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info_.surface          = physicalDevice.get_surface();
	swapchain_info_.minImageCount    = get_image_count();
	swapchain_info_.imageFormat      = format.format;
	swapchain_info_.imageColorSpace  = format.colorSpace;
	swapchain_info_.imageExtent      = extent;
	swapchain_info_.imageArrayLayers = 1;
	swapchain_info_.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	const uint32_t queueFamilyIndices[] = {physicalDevice.graphics.value(), physicalDevice.present.value()};
	if(physicalDevice.graphics != physicalDevice.present)
	{
		swapchain_info_.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
		swapchain_info_.queueFamilyIndexCount = 2;
		swapchain_info_.pQueueFamilyIndices   = queueFamilyIndices;
	}
	else
	{
		swapchain_info_.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
		swapchain_info_.queueFamilyIndexCount = 0;
		swapchain_info_.pQueueFamilyIndices   = nullptr;
	}

	swapchain_info_.preTransform   = capabilities.currentTransform;
	swapchain_info_.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchain_info_.presentMode    = get_present_mode();
	swapchain_info_.clipped        = true;
	swapchain_info_.oldSwapchain   = VK_NULL_HANDLE;

	VkSwapchainKHR swapchain;
	if(vkCreateSwapchainKHR(device_, &swapchain_info_, nullptr, &swapchain) != VK_SUCCESS)
		throw std::runtime_error("failed to create swapchain");

	return {swapchain, device_, format.format, extent};
}

SwapchainBuilder& SwapchainBuilder::set_desired_format(VkSurfaceFormatKHR format) noexcept
{
	info_.desired_format = format;
	return *this;
}

SwapchainBuilder& SwapchainBuilder::set_desired_present_mode(VkPresentModeKHR presentMode) noexcept
{
	info_.desired_present_mode = presentMode;
	return *this;
}

SwapchainBuilder& SwapchainBuilder::set_desired_extent(VkExtent2D extent) noexcept
{
	info_.desired_extent = extent;
	return *this;
}

uint32_t SwapchainBuilder::get_image_count() const
{
	const auto capabilities = device_.get_physical_device().get_surface_capabilities();
	const auto imageCount   = capabilities.minImageCount + 1;
	if(capabilities.maxImageCount != 0 && imageCount > capabilities.maxImageCount)
		return capabilities.maxImageCount;
	return imageCount;
}

VkSurfaceFormatKHR SwapchainBuilder::get_surface_format() const
{
	const auto         formats = device_.get_physical_device().get_surface_formats();
	VkSurfaceFormatKHR format  = formats[0];
	for(const auto& fmt : formats)
		if(fmt.format == info_.desired_format.format && fmt.colorSpace == info_.desired_format.colorSpace)
			format = fmt;
	return format;
}

VkPresentModeKHR SwapchainBuilder::get_present_mode() const
{
	const auto       presentModes = device_.get_physical_device().get_surface_present_modes();
	VkPresentModeKHR presentMode  = VK_PRESENT_MODE_FIFO_KHR;
	for(auto mode : presentModes)
		if(mode == info_.desired_present_mode)
			presentMode = mode;
	return presentMode;
}

VkExtent2D SwapchainBuilder::get_extent() const
{
	const auto capabilities = device_.get_physical_device().get_surface_capabilities();
	auto       extent       = capabilities.currentExtent;
	if(capabilities.currentExtent.width == -1)
	{
		extent.width =
		    std::clamp(info_.desired_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
		extent.height = std::clamp(info_.desired_extent.height, capabilities.minImageExtent.height,
		                           capabilities.maxImageExtent.height);
	}
	return extent;
}
