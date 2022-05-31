// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "InstanceBuilder.h"
#include "../VKInstance.h"
#include "Core/Platform.h"
#include <stdexcept>

namespace
{

std::string_view toString(VkDebugUtilsMessageSeverityFlagBitsEXT severity)
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
	}
	return "";
}

std::string_view toString(VkDebugUtilsMessageTypeFlagsEXT type)
{
	switch(type)
	{
	case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
		return "general";

	case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
		return "validation";

	case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
		return "performance";
	}
	return "";
}

}

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

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;

	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.enabledLayerCount = static_cast<uint32_t>(enabledLayers.size());
	instanceInfo.ppEnabledLayerNames = enabledLayers.data();
	instanceInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
	instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();

	VkInstance instance;
	if(vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
		throw std::runtime_error("failed to create instance");

	if(info.enableDebugMessager)
		createDebugMessager(instance);

	return instance;
}

InstanceBuilder& InstanceBuilder::setEngineName(std::string_view name)
{
	appInfo.pEngineName = name.data();
	return *this;
}

InstanceBuilder& InstanceBuilder::setEngineVersion(uint32_t major, uint32_t minor, uint32_t patch)
{
	appInfo.engineVersion = VK_MAKE_VERSION(major, minor, patch);
	return *this;
}

InstanceBuilder& InstanceBuilder::setAppName(std::string_view name)
{
	appInfo.pApplicationName = name.data();
	return *this;
}

InstanceBuilder& InstanceBuilder::setAppVersion(uint32_t major, uint32_t minor, uint32_t patch)
{
	appInfo.applicationVersion = VK_MAKE_VERSION(major, minor, patch);
	return *this;
}

InstanceBuilder& InstanceBuilder::enableLayer(std::string_view name)
{
	if(!isLayerAvailable(name))
		throw std::runtime_error("requested layer not present");
	enabledLayers.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enableExtension(std::string_view name)
{
	if(!isExtensionAvailable(name))
		throw std::runtime_error("requested extension not present");
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

InstanceBuilder& InstanceBuilder::setDebugCallback(PFN_vkDebugUtilsMessengerCallbackEXT callback)
{
	info.debugMessageCallback = callback;
	return *this;
}

bool InstanceBuilder::isLayerAvailable(std::string_view name) const
{
	for(const auto& layer : availableLayers)
		if(layer.layerName == name)
			return true;
	return false;
}

bool InstanceBuilder::isExtensionAvailable(std::string_view name) const
{
	for(const auto& ext : availableExtensions)
		if(ext.extensionName == name)
			return true;
	return false;
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

	auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if(vkCreateDebugUtilsMessengerEXT == nullptr)
		throw std::runtime_error("failed to locate function");

	const auto defaultCallback = [](
		VkDebugUtilsMessageSeverityFlagBitsEXT severity,
		VkDebugUtilsMessageTypeFlagsEXT type,
		const VkDebugUtilsMessengerCallbackDataEXT* data,
		void* pUserData)
	{
		printf("[%s: %s]\n%s\n", toString(severity).data(), toString(type).data(), data->pMessage);
		return VK_FALSE;
	};

	VkDebugUtilsMessengerCreateInfoEXT messagerInfo = {};
	messagerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
	messagerInfo.messageSeverity = info.debugMessageSeverity;
	messagerInfo.messageType = info.debugMessageType;
	messagerInfo.pfnUserCallback = info.debugMessageCallback ? info.debugMessageCallback : defaultCallback;

	if(vkCreateDebugUtilsMessengerEXT(instance, &messagerInfo, nullptr, &debugMessenger) != VK_SUCCESS)
		throw std::runtime_error("failed to create debug utils messager");
}

void InstanceBuilder::destroyDebugMessager(VkInstance instance)
{
	auto vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if(vkDestroyDebugUtilsMessengerEXT == nullptr)
		throw std::runtime_error("failed to locate function");

	vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
}
