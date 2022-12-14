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
	SwapchainBuilder& setDesiredFormat(VkSurfaceFormatKHR format) noexcept;

	/**
	 * @brief 设置期望的呈现模式.
	 *
	 * @param presentMode 期望的呈现模式.
	 */
	SwapchainBuilder& setDesiredPresentMode(VkPresentModeKHR presentMode) noexcept;

	/**
	 * @brief 设置期望的面积, 即帧缓冲区大小.
	 *
	 * @param extent 期望的面积.
	 */
	SwapchainBuilder& setDesiredExtent(VkExtent2D extent) noexcept;

private:
	uint32_t           getImageCount() const;
	VkSurfaceFormatKHR getSurfaceFormat() const;
	VkPresentModeKHR   getPresentMode() const;
	VkExtent2D         getExtent() const;

	VkSwapchainCreateInfoKHR swapchainInfo = {};

	VKDevice& device;

	struct
	{
		VkSurfaceFormatKHR desiredFormat      = {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
		VkPresentModeKHR   desiredPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
		VkExtent2D         desiredExtent      = {};
	} info;
};
