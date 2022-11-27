// Copyright 2021 ShenMian
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
	availableLayers.resize(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// 获取可用拓展
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
	availableExtensions.resize(extensionCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
}

VKInstance InstanceBuilder::build()
{
	enableWindowExtensions();

	if(std::ranges::any_of(enabledLayers, [this](auto layer) { return !isLayerAvailable(layer); }))
		throw std::runtime_error("requested layer not present");

	if(std::ranges::any_of(enabledExtensions, [this](auto ext) { return !isExtensionAvailable(ext); }))
		throw std::runtime_error("requested extension not avaliable");

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	instanceInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo        = &appInfo;
	instanceInfo.enabledLayerCount       = static_cast<uint32_t>(enabledLayers.size());
	instanceInfo.ppEnabledLayerNames     = enabledLayers.data();
	instanceInfo.enabledExtensionCount   = static_cast<uint32_t>(enabledExtensions.size());
	instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();

	VkInstance instance;
	if(vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance");

	if(info.enableDebugMessager)
		createDebugMessager(instance);

	return instance;
}

InstanceBuilder& InstanceBuilder::setEngineName(std::string_view name) noexcept
{
	appInfo.pEngineName = name.data();
	return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(uint32_t major, uint32_t minor, uint32_t patch) noexcept
{
	appInfo.engineVersion = VK_MAKE_VERSION(major, minor, patch);
	return *this;
}

InstanceBuilder& InstanceBuilder::setAppName(std::string_view name) noexcept
{
	appInfo.pApplicationName = name.data();
	return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(uint32_t major, uint32_t minor, uint32_t patch) noexcept
{
	appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);
	return *this;
}

InstanceBuilder& InstanceBuilder::enableLayer(std::string_view name)
{
	enabledLayers.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enableExtension(std::string_view name)
{
	enabledExtensions.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enableValidationLayers()
{
	info.enableValidationLayers = true;
	enableLayer("VK_LAYER_KHRONOS_validation");
	return *this;
}

InstanceBuilder& InstanceBuilder::enableDebugMessenger()
{
	info.enableDebugMessager = true;
	enableExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	return *this;
}

InstanceBuilder& InstanceBuilder::setDebugCallback(PFN_vkDebugUtilsMessengerCallbackEXT callback) noexcept
{
	info.debugMessageCallback = callback;
	return *this;
}

bool InstanceBuilder::isLayerAvailable(std::string_view name) const
{
	return std::ranges::any_of(availableLayers, [name](const auto& layer) { return layer.layerName == name; });
}

bool InstanceBuilder::isExtensionAvailable(std::string_view name) const
{
	return std::ranges::any_of(availableExtensions, [name](const auto& ext) { return ext.extensionName == name; });
}

void InstanceBuilder::enableWindowExtensions()
{
	enableExtension("VK_KHR_surface");
#if TARGET_OS == OS_WIN
	enableExtension("VK_KHR_win32_surface");
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

void InstanceBuilder::createDebugMessager(VkInstance instance)
{
	if(!info.enableValidationLayers)
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
	messagerInfo.messageSeverity                    = info.debugMessageSeverity;
	messagerInfo.messageType                        = info.debugMessageType;
	messagerInfo.pfnUserCallback = info.debugMessageCallback ? info.debugMessageCallback : defaultCallback;

	if(vkCreateDebugUtilsMessengerEXT(instance, &messagerInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to create debug utils messager");
}

void InstanceBuilder::destroyDebugMessager(VkInstance instance)
{
	const auto vkDestroyDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
	    vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"));
	if(vkDestroyDebugUtilsMessengerEXT == nullptr)
		throw std::runtime_error("failed to locate function");

	vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
}
