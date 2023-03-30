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
	InstanceBuilder& set_engine_name(std::string_view name) noexcept;

	/**
	 * @brief 设置引擎版本. (可选)
	 *
	 * @param major 引擎主版本.
	 * @param minor 引擎子版本.
	 * @param patch 引擎阶段版本.
	 */
	InstanceBuilder& set_engine_version(uint32_t major, uint32_t minor, uint32_t patch) noexcept;

	/**
	 * @brief 设置应用程序名称. (可选)
	 *
	 * @param name 应用程序名称.
	 */
	InstanceBuilder& set_app_name(std::string_view name) noexcept;

	/**
	 * @brief 设置应用程序版本. (可选)
	 *
	 * @param major 应用程序主版本.
	 * @param minor 应用程序子版本.
	 * @param patch 应用程序阶段版本.
	 */
	InstanceBuilder& set_app_version(uint32_t major, uint32_t minor, uint32_t patch) noexcept;

	/**
	 * @brief 启用 Layer.
	 *
	 * @param name Layer 名称.
	 */
	InstanceBuilder& enable_layer(std::string_view name);

	/**
	 * @brief 启用拓展.
	 *
	 * @param name 拓展名称.
	 */
	InstanceBuilder& enable_extension(std::string_view name);

	/**
	 * @brief 启用验证层.
	 */
	InstanceBuilder& enable_validation_layers();

	/**
	 * @brief 启用调试消息.
	 */
	InstanceBuilder& enable_debug_messenger();

	/**
	 * @brief 设置调试消息回调函数.
	 *
	 * @param callback 回调函数.
	 */
	InstanceBuilder& set_debug_callback(PFN_vkDebugUtilsMessengerCallbackEXT callback) noexcept;

private:
	bool is_layer_available(std::string_view name) const;
	bool is_extension_available(std::string_view name) const;

	void enable_window_extensions();
	void create_debug_messager(VkInstance instance);
	void destroy_debug_messager(VkInstance instance);

	VkApplicationInfo    app_info_      = {};
	VkInstanceCreateInfo instance_info_ = {};

	VkDebugUtilsMessengerEXT debug_messenger_;

	std::vector<const char*> enabled_layers_;
	std::vector<const char*> enabled_extensions_;

	std::vector<VkLayerProperties>     available_layers_;
	std::vector<VkExtensionProperties> available_extensions_;

	struct
	{
		bool enable_validation_layers = false;

		bool                                enable_debug_messager = false;
		VkDebugUtilsMessageSeverityFlagsEXT debug_message_severity =
		    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		VkDebugUtilsMessageTypeFlagsEXT debug_message_type = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
		                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
		                                                   VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		PFN_vkDebugUtilsMessengerCallbackEXT debug_message_callback = nullptr;
	} info_;
};
