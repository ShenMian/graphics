// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "../VKInstance.h"
#include <string_view>
#include <vector>

class InstanceBuilder final
{
public:
	InstanceBuilder();

	VKInstance build();

	/**
	 * @brief 设置引擎名称. (可选)
	 *
	 * @param name 引擎名称.
	 */
	InstanceBuilder& setEngineName(std::string_view name) noexcept;

	/**
	 * @brief 设置引擎版本. (可选)
	 *
	 * @param major 引擎主版本.
	 * @param minor 引擎子版本.
	 * @param patch 引擎阶段版本.
	 */
	InstanceBuilder& setEngineVersion(uint32_t major, uint32_t minor, uint32_t patch) noexcept;

	/**
	 * @brief 设置应用程序名称. (可选)
	 *
	 * @param name 应用程序名称.
	 */
	InstanceBuilder& setAppName(std::string_view name) noexcept;

	/**
	 * @brief 设置应用程序版本. (可选)
	 *
	 * @param major 应用程序主版本.
	 * @param minor 应用程序子版本.
	 * @param patch 应用程序阶段版本.
	 */
	InstanceBuilder& setAppVersion(uint32_t major, uint32_t minor, uint32_t patch) noexcept;

	/**
	 * @brief 启用 Layer.
	 *
	 * @param name Layer 名称.
	 */
	InstanceBuilder& enableLayer(std::string_view name);

	/**
	 * @brief 启用拓展.
	 *
	 * @param name 拓展名称.
	 */
	InstanceBuilder& enableExtension(std::string_view name);

	/**
	 * @brief 启用验证层.
	 */
	InstanceBuilder& enableValidationLayers();

	/**
	 * @brief 启用调试消息.
	 */
	InstanceBuilder& enableDebugMessenger();

	/**
	 * @brief 设置调试消息回调函数.
	 *
	 * @param callback 回调函数.
	 */
	InstanceBuilder& setDebugCallback(PFN_vkDebugUtilsMessengerCallbackEXT callback) noexcept;

private:
	bool isLayerAvailable(std::string_view name) const;
	bool isExtensionAvailable(std::string_view name) const;

	void enableWindowExtensions();
	void createDebugMessager(VkInstance instance);
	void destroyDebugMessager(VkInstance instance);

	VkApplicationInfo    appInfo      = {};
	VkInstanceCreateInfo instanceInfo = {};

	VkDebugUtilsMessengerEXT debugMessenger;

	std::vector<const char*> enabledLayers;
	std::vector<const char*> enabledExtensions;

	std::vector<VkLayerProperties>     availableLayers;
	std::vector<VkExtensionProperties> availableExtensions;

	struct
	{
		bool enableValidationLayers = false;

		bool                                enableDebugMessager = false;
		VkDebugUtilsMessageSeverityFlagsEXT debugMessageSeverity =
		    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		VkDebugUtilsMessageTypeFlagsEXT debugMessageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		PFN_vkDebugUtilsMessengerCallbackEXT debugMessageCallback = nullptr;
	} info;
};
