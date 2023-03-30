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

	const VkPhysicalDeviceProperties& get_properties() const noexcept;

	const VkPhysicalDeviceFeatures& get_features() const noexcept;

	VkSurfaceKHR get_surface() const noexcept;

	/**
	 * @brief 获取设备名称.
	 */
	std::string_view get_name() const noexcept;

	/**
	 * @brief 获取设备供应商名称.
	 */
	std::string_view get_vendor_name() const noexcept;

	VkSurfaceCapabilitiesKHR        get_surface_capabilities() const;
	std::vector<VkSurfaceFormatKHR> get_surface_formats() const;
	std::vector<VkPresentModeKHR>   get_surface_present_modes() const;

	bool is_extension_available(std::string_view name) const;

	std::optional<uint32_t> graphics = std::nullopt;
	std::optional<uint32_t> compute  = std::nullopt;
	std::optional<uint32_t> transfer = std::nullopt;
	std::optional<uint32_t> present  = std::nullopt;

	operator VkPhysicalDevice();
	operator VkPhysicalDevice() const;

private:
	VkPhysicalDevice           handle_;
	VkPhysicalDeviceProperties properties_;
	VkPhysicalDeviceFeatures   features_;

	VkSurfaceKHR surface_;

	std::vector<VkExtensionProperties> available_extensions_;
};
