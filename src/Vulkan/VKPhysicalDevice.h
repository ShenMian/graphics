// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <optional>
#include <string_view>
#include <vector>
#include <vulkan/vulkan.h>

/**
 * @brief Vulkan 物理设备.
 */
class VKPhysicalDevice final
{
public:
	VKPhysicalDevice() = default;

	VKPhysicalDevice(VkPhysicalDevice device, VkSurfaceKHR surface);

	const VkPhysicalDeviceProperties& getProperties() const noexcept;

	const VkPhysicalDeviceFeatures& getFeatures() const noexcept;

	VkSurfaceKHR getSurface() const noexcept;

	/**
	 * @brief 获取设备名称.
	 */
	std::string_view getName() const noexcept;

	/**
	 * @brief 获取设备供应商名称.
	 */
	std::string_view getVendorName() const noexcept;

	VkSurfaceCapabilitiesKHR        getSurfaceCapabilities() const;
	std::vector<VkSurfaceFormatKHR> getSurfaceFormats() const;
	std::vector<VkPresentModeKHR>   getSurfacePresentModes() const;

	bool isExtensionAvailable(std::string_view name) const;

	std::optional<uint32_t> graphics = std::nullopt;
	std::optional<uint32_t> compute  = std::nullopt;
	std::optional<uint32_t> transfer = std::nullopt;
	std::optional<uint32_t> present  = std::nullopt;

	operator VkPhysicalDevice();
	operator VkPhysicalDevice() const;

private:
	VkPhysicalDevice           handle;
	VkPhysicalDeviceProperties properties;
	VkPhysicalDeviceFeatures   features;

	VkSurfaceKHR surface;

	std::vector<VkExtensionProperties> availableExtensions;
};
