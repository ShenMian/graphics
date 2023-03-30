// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "InstanceBuilder.h"
#include "../VKInstance.h"
#include "Core/Platform.h"
#include <algorithm>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace
{

std::string_view ToString(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
{
	switch(severity)
	{
	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
		return "error";

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
		return "warning";

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
		return "info";

	case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
		return "verbose";

	default:
		throw std::runtime_error("unknown debug message severity");
	}
}

std::string_view ToString(VkDebugUtilsMessageTypeFlagsEXT type)
{
	switch(type)
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		return "general";

	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		return "validation";

	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		return "performance";

	default:
		throw std::runtime_error("unknown debug message type");
	}
}

} // namespace

InstanceBuilder::InstanceBuilder()
{
	// 获取可用 Layer
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	available_layers_.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, available_layers_.data());

	// 获取可用拓展
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	available_extensions_.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, available_extensions_.data());
}

VKInstance InstanceBuilder::build()
{
	enable_window_extensions();

	if(std::ranges::any_of(enabled_layers_, [this](auto layer) { return !is_layer_available(layer); }))
		throw std::runtime_error("requested layer is not available");

	if(std::ranges::any_of(enabled_extensions_, [this](auto ext) { return !is_extension_available(ext); }))
		throw std::runtime_error("requested extension is not available");

	app_info_.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	instance_info_.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instance_info_.pApplicationInfo        = &app_info_;
	instance_info_.enabledLayerCount       = static_cast<uint32_t>(enabled_layers_.size());
	instance_info_.ppEnabledLayerNames     = enabled_layers_.data();
	instance_info_.enabledExtensionCount   = static_cast<uint32_t>(enabled_extensions_.size());
	instance_info_.ppEnabledExtensionNames = enabled_extensions_.data();

	VkInstance instance;
	if(vkCreateInstance(&instance_info_, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance");

	if(info_.enable_debug_messager)
		create_debug_messager(instance);

	return instance;
}

InstanceBuilder& InstanceBuilder::set_engine_name(std::string_view name) noexcept
{
	app_info_.pEngineName = name.data();
	return *this;
}

InstanceBuilder& InstanceBuilder::set_engine_version(uint32_t major, uint32_t minor, uint32_t patch) noexcept
{
	app_info_.engineVersion = VK_MAKE_VERSION(major, minor, patch);
	return *this;
}

InstanceBuilder& InstanceBuilder::set_app_name(std::string_view name) noexcept
{
	app_info_.pApplicationName = name.data();
	return *this;
}

InstanceBuilder& InstanceBuilder::set_app_version(uint32_t major, uint32_t minor, uint32_t patch) noexcept
{
	app_info_.applicationVersion = VK_MAKE_VERSION(major, minor, patch);
	return *this;
}

InstanceBuilder& InstanceBuilder::enable_layer(std::string_view name)
{
	enabled_layers_.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enable_extension(std::string_view name)
{
	enabled_extensions_.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enable_validation_layers()
{
	info_.enable_validation_layers = true;
	enable_layer("VK_LAYER_KHRONOS_validation");
	return *this;
}

InstanceBuilder& InstanceBuilder::enable_debug_messenger()
{
	info_.enable_debug_messager = true;
	enable_extension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	return *this;
}

InstanceBuilder& InstanceBuilder::set_debug_callback(PFN_vkDebugUtilsMessengerCallbackEXT callback) noexcept
{
	info_.debug_message_callback = callback;
	return *this;
}

bool InstanceBuilder::is_layer_available(std::string_view name) const
{
	return std::ranges::any_of(available_layers_, [name](const auto& layer) { return layer.layerName == name; });
}

bool InstanceBuilder::is_extension_available(std::string_view name) const
{
	return std::ranges::any_of(available_extensions_, [name](const auto& ext) { return ext.extensionName == name; });
}

void InstanceBuilder::enable_window_extensions()
{
	enable_extension("VK_KHR_surface");
#if TARGET_OS == OS_WIN
	enable_extension("VK_KHR_win32_surface");
#elif TARGET_OS == OS_ANDROID
	enableExtension("VK_KHR_android_surface");
#elif TARGET_OS == OS_LINUX
	enableExtension("VK_KHR_xcb_surface");
	enableExtension("VK_KHR_xlib_surface");
	enableExtension("VK_KHR_wayland_surface");
#elif TARGET_OS == OS_MAC || TARGET_OS == OS_IOS
	enableExtension("VK_EXT_metal_surface");
#elif defined(_DIRECT2DISPLAY)
	enableExtension("VK_KHR_display");
#endif
}

void InstanceBuilder::create_debug_messager(VkInstance instance)
{
	if(!info_.enable_validation_layers)
		throw std::runtime_error("must enable validation layers");

	const auto vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
	    vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT"));
	if(vkCreateDebugUtilsMessengerEXT == nullptr)
		throw std::runtime_error("failed to locate function");

	const auto defaultCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
	                                VkDebugUtilsMessageTypeFlagsEXT             type,
	                                const VkDebugUtilsMessengerCallbackDataEXT* data, void* pUserData) {
		puts(fmt::format("Vulkan Message\n"
		                 "|-Type:     {}\n"
		                 "|-Severity: {}\n"
		                 "`-Message:  {}",
		                 ToString(type), ToString(severity), data->pMessage)
		         .c_str());
		return VK_FALSE;
	};

	VkDebugUtilsMessengerCreateInfoEXT messagerInfo = {};
	messagerInfo.sType                              = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messagerInfo.messageSeverity                    = info_.debug_message_severity;
	messagerInfo.messageType                        = info_.debug_message_type;
	messagerInfo.pfnUserCallback = info_.debug_message_callback ? info_.debug_message_callback : defaultCallback;

	if(vkCreateDebugUtilsMessengerEXT(instance, &messagerInfo, nullptr, &debug_messenger_) != VK_SUCCESS)
		throw std::runtime_error("failed to create debug utils messager");
}

void InstanceBuilder::destroy_debug_messager(VkInstance instance)
{
	const auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
	    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	if(vkDestroyDebugUtilsMessengerEXT == nullptr)
		throw std::runtime_error("failed to locate function");

	vkDestroyDebugUtilsMessengerEXT(instance, debug_messenger_, nullptr);
}
