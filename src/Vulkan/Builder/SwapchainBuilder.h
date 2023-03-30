// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKDevice.h"
#include "../VKSwapchain.h"

class SwapchainBuilder
{
public:
	SwapchainBuilder(VKDevice& device);

	VKSwapchain build();

	/**
	 * @brief 设置期望的格式.
	 *
	 * @param format 期望的格式.
	 */
	SwapchainBuilder& set_desired_format(VkSurfaceFormatKHR format) noexcept;

	/**
	 * @brief 设置期望的呈现模式.
	 *
	 * @param presentMode 期望的呈现模式.
	 */
	SwapchainBuilder& set_desired_present_mode(VkPresentModeKHR presentMode) noexcept;

	/**
	 * @brief 设置期望的面积, 即帧缓冲区大小.
	 *
	 * @param extent 期望的面积.
	 */
	SwapchainBuilder& set_desired_extent(VkExtent2D extent) noexcept;

private:
	uint32_t           get_image_count() const;
	VkSurfaceFormatKHR get_surface_format() const;
	VkPresentModeKHR   get_present_mode() const;
	VkExtent2D         get_extent() const;

	VkSwapchainCreateInfoKHR swapchain_info_ = {};

	VKDevice& device_;

	struct
	{
		VkSurfaceFormatKHR desired_format      = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		VkPresentModeKHR   desired_present_mode = VK_PRESENT_MODE_MAILBOX_KHR;
		VkExtent2D         desired_extent      = {};
	} info_;
};
