// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKRenderer.h"
#include "Builder/DeviceBuilder.h"
#include "Builder/InstanceBuilder.h"
#include "Builder/PhysicalDeviceSelector.h"
#include "Builder/SwapchainBuilder.h"
#include <GLFW/glfw3.h>

namespace
{

VKInstance       instance;
VkSurfaceKHR     surface;
VKPhysicalDevice physicalDevice;
VKDevice         device;
VKSwapchain      swapchain;
VkCommandPool    commandPool;

} // namespace

std::string VKRenderer::getDeviceName() const
{
	return std::string(physicalDevice.getName());
}

std::string VKRenderer::getRendererName() const
{
	const auto version = physicalDevice.getProperties().apiVersion;
	return "Vulkan " + std::to_string(VK_VERSION_MAJOR(version)) + '.' + std::to_string(VK_VERSION_MINOR(version)) +
	       '.' + std::to_string(VK_VERSION_PATCH(version));
}

std::string VKRenderer::getVendorName() const
{
	return std::string(physicalDevice.getVendorName());
}

const VKInstance& VKRenderer::getInstance() const
{
	return instance;
}

const VKPhysicalDevice& VKRenderer::getPhysicalDevice() const
{
	return physicalDevice;
}

const VKDevice& VKRenderer::getDevice() const
{
	return device;
}

const VKSwapchain& VKRenderer::getSwapchain() const
{
	return swapchain;
}

const VkCommandPool& VKRenderer::getCommandPool() const
{
	return commandPool;
}

void VKRenderer::init(const Window& win)
{
	createInstance();
	createSurface(win);
	selectPhysicalDevice();
	createDevice();
	createSwapchain();
	createCommandPool();
}

void VKRenderer::deinit()
{
	vkDestroyCommandPool(device, commandPool, nullptr);
	swapchain.destroy();
	device.destroy();
	vkDestroySurfaceKHR(instance, surface, nullptr);
	instance.destroy();
}

void VKRenderer::createInstance()
{
	InstanceBuilder builder;
	instance = builder.enableValidationLayers().enableDebugMessenger().build();
}

void VKRenderer::createSurface(const Window& win)
{
	if(glfwCreateWindowSurface(instance, win.getHandle(), nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
}

void VKRenderer::selectPhysicalDevice()
{
	PhysicalDeviceSelector selector(instance, surface);
	physicalDevice = selector.requireGraphicsQueue().requirePresentQueue().requireTransferQueue().select();
}

void VKRenderer::createDevice()
{
	DeviceBuilder builder(physicalDevice);
	device = builder.build();
}

void VKRenderer::createSwapchain()
{
	SwapchainBuilder builder(device);
	swapchain = builder.build();
}

void VKRenderer::createCommandPool()
{
	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex        = device.getQueueIndex(VKDevice::QueueType::Graphics);
	;
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	if(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool");
}
