// Copyright 2022 ShenMian
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

std::string VKRenderer::get_device_name() const
{
	return std::string(physicalDevice.get_name());
}

std::string VKRenderer::get_renderer_name() const
{
	const auto version = physicalDevice.get_properties().apiVersion;
	return "Vulkan " + std::to_string(VK_VERSION_MAJOR(version)) + '.' + std::to_string(VK_VERSION_MINOR(version)) +
	       '.' + std::to_string(VK_VERSION_PATCH(version));
}

std::string VKRenderer::get_vendor_name() const
{
	return std::string(physicalDevice.get_vendor_name());
}

const VKInstance& VKRenderer::get_instance() const
{
	return instance;
}

const VKPhysicalDevice& VKRenderer::get_physical_device() const
{
	return physicalDevice;
}

const VKDevice& VKRenderer::get_device() const
{
	return device;
}

const VKSwapchain& VKRenderer::get_swapchain() const
{
	return swapchain;
}

const VkCommandPool& VKRenderer::get_command_pool() const
{
	return commandPool;
}

void VKRenderer::init(const Window& win)
{
	create_instance();
	create_surface(win);
	select_physical_device();
	create_device();
	create_swapchain();
	create_command_pool();
}

void VKRenderer::deinit()
{
	vkDestroyCommandPool(device, commandPool, nullptr);
	swapchain.destroy();
	device.destroy();
	vkDestroySurfaceKHR(instance, surface, nullptr);
	instance.destroy();
}

void VKRenderer::create_instance()
{
	InstanceBuilder builder;
	instance = builder.enable_validation_layers().enable_debug_messenger().build();
}

void VKRenderer::create_surface(const Window& win)
{
	if(glfwCreateWindowSurface(instance, win.get_handle(), nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
}

void VKRenderer::select_physical_device()
{
	PhysicalDeviceSelector selector(instance, surface);
	physicalDevice = selector.require_graphics_queue().require_present_queue().require_transfer_queue().select();
}

void VKRenderer::create_device()
{
	DeviceBuilder builder(physicalDevice);
	device = builder.build();
}

void VKRenderer::create_swapchain()
{
	SwapchainBuilder builder(device);
	swapchain = builder.build();
}

void VKRenderer::create_command_pool()
{
	VkCommandPoolCreateInfo commandPoolInfo = {};
	commandPoolInfo.sType                   = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex        = device.get_queue_index(VKDevice::QueueType::Graphics).value();
	commandPoolInfo.flags                   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	if(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool");
}
