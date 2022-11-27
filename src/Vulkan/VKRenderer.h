// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Renderer.h"
#include "VKDevice.h"
#include "VKInstance.h"
#include "VKPhysicalDevice.h"
#include "VKSwapchain.h"
#include <vulkan/vulkan.h>

class VKRenderer : public Renderer
{
public:
	std::string getDeviceName() const override;
	std::string getRendererName() const override;
	std::string getVendorName() const override;

	/**
	 * @brief 获取 VKInstance.
	 */
	const VKInstance& getInstance() const;

	/**
	 * @brief 获取 VKPhysicalDevice.
	 */
	const VKPhysicalDevice& getPhysicalDevice() const;

	/**
	 * @brief 获取 VKDevice.
	 */
	const VKDevice& getDevice() const;

	/**
	 * @brief 获取 VKSwapchain.
	 */
	const VKSwapchain& getSwapchain() const;

	/**
	 * @brief 获取命令缓冲区池.
	 */
	const VkCommandPool& getCommandPool() const;

	static void init(const Window& win);
	static void deinit();

private:
	static void createInstance();
	static void createSurface(const Window& win);
	static void selectPhysicalDevice();
	static void createDevice();
	static void createSwapchain();
	static void createCommandPool();
};

inline std::string_view ToString(VkResult res)
{
	switch(res)
	{
	case VK_SUCCESS:
		return "VK_SUCCESS";

	case VK_NOT_READY:
		return "VK_NOT_READY";

	case VK_TIMEOUT:
		return "VK_TIMEOUT";

	case VK_EVENT_SET:
		return "VK_EVENT_SET";

	case VK_EVENT_RESET:
		return "VK_EVENT_RESET";

	case VK_INCOMPLETE:
		return "VK_INCOMPLETE";

	case VK_ERROR_OUT_OF_HOST_MEMORY:
		return "VK_ERROR_OUT_OF_HOST_MEMORY";

	case VK_ERROR_OUT_OF_DEVICE_MEMORY:
		return "VK_ERROR_OUT_OF_DEVICE_MEMORY";

	case VK_ERROR_INITIALIZATION_FAILED:
		return "VK_ERROR_INITIALIZATION_FAILED";

	case VK_ERROR_DEVICE_LOST:
		return "VK_ERROR_DEVICE_LOST";

	case VK_ERROR_MEMORY_MAP_FAILED:
		return "VK_ERROR_MEMORY_MAP_FAILED";

	case VK_ERROR_LAYER_NOT_PRESENT:
		return "VK_ERROR_LAYER_NOT_PRESENT";

	case VK_ERROR_FEATURE_NOT_PRESENT:
		return "VK_ERROR_FEATURE_NOT_PRESENT";

	case VK_ERROR_INCOMPATIBLE_DRIVER:
		return "VK_ERROR_INCOMPATIBLE_DRIVER";

	case VK_ERROR_TOO_MANY_OBJECTS:
		return "VK_ERROR_TOO_MANY_OBJECTS";

	case VK_ERROR_FORMAT_NOT_SUPPORTED:
		return "VK_ERROR_FORMAT_NOT_SUPPORTED";

	case VK_ERROR_SURFACE_LOST_KHR:
		return "VK_ERROR_SURFACE_LOST_KHR";

	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
		return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";

	case VK_SUBOPTIMAL_KHR:
		return "VK_SUBOPTIMAL_KHR";

	case VK_ERROR_OUT_OF_DATE_KHR:
		return "VK_ERROR_OUT_OF_DATE_KHR";

	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
		return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";

	case VK_ERROR_VALIDATION_FAILED_EXT:
		return "VK_ERROR_VALIDATION_FAILED_EXT";

	case VK_ERROR_INVALID_SHADER_NV:
		return "VK_ERROR_INVALID_SHADER_NV";

	default:
		return "Unknown";
	}
}

inline VkResult VKCheck(VkResult res)
{
	if(res != VK_SUCCESS)
		throw std::runtime_error("Vulkan:" + std::string(ToString(res)));
	return res;
}
