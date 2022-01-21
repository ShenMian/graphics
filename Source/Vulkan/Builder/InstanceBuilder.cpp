// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "InstanceBuilder.h"
#include "../VKInstance.h"
#include <stdexcept>

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

	if(info.useDebugMessager)
	{
		if(!info.enableValidationLayers)
			throw std::runtime_error("must enable validation layers");

		auto VkCreateDebugUtilsMessager = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if(VkCreateDebugUtilsMessager == nullptr)
			throw std::runtime_error("failed to locate function");

		const auto callback = [](
			VkDebugUtilsMessageSeverityFlagBitsEXT severity,
			VkDebugUtilsMessageTypeFlagsEXT type,
			const VkDebugUtilsMessengerCallbackDataEXT* data,
			void* pUserData)
		{
			puts(data->pMessage);
			return VK_FALSE;
		};

		VkDebugUtilsMessengerCreateInfoEXT messagerInfo = {};
		messagerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messagerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messagerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messagerInfo.pfnUserCallback = callback;

		VkDebugUtilsMessengerEXT debugMessager;
		if(VkCreateDebugUtilsMessager(instance, &messagerInfo, nullptr, &debugMessager) != VK_SUCCESS)
			throw std::runtime_error("failed to create debug utils messager");
	}

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
	if(isLayerAvailable(name))
		enabledLayers.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enableExtension(std::string_view name)
{
	if(isExtensionAvailable(name))
		enabledExtensions.emplace_back(name.data());
	return *this;
}

InstanceBuilder& InstanceBuilder::enableValidationLayers()
{
	info.enableValidationLayers = true;
	enableLayer("VK_LAYER_KHRONOS_validation");
	return *this;
}

InstanceBuilder& InstanceBuilder::useDebugMessager()
{
	info.useDebugMessager = true;
	enableLayer(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
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
