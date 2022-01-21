// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <vulkan/vulkan.h>
#include <string_view>
#include <vector>

class VKPhysicalDevice final
{
public:
	VKPhysicalDevice() = default;

	VKPhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface);

	const VkPhysicalDeviceProperties& getProperties() const;

	const VkPhysicalDeviceFeatures& getFeatures() const;

	VkSurfaceKHR getSurface() const;

	/**
	 * @brief 获取设备名称.
	 */
	std::string_view getName() const;

	/**
	 * @brief 获取设备供应商名称.
	 */
	std::string_view getVendorName() const;

	bool isExtensionAvailable(std::string_view name) const;

	uint32_t graphics = -1;
	uint32_t compute = -1;
	uint32_t transfer = -1;
	uint32_t present = -1;

	operator VkPhysicalDevice();
	operator VkPhysicalDevice() const;

private:
	VkPhysicalDevice           handle;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures   features;

	VkSurfaceKHR surface;

	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkExtensionProperties> availableExtensions;
};
