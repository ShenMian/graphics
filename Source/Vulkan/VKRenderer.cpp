// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKRenderer.h"
#include "Builder/InstanceBuilder.h"
#include "Builder/PhysicalDeviceSelector.h"
#include "Builder/DeviceBuilder.h"
#include "Builder/SwapchainBuilder.h"

#include <VkBootstrap.h>
#include <GLFW/glfw3.h>

namespace
{

VKInstance       instance;
VkSurfaceKHR     surface;
VKPhysicalDevice physicalDevice;
VKDevice         device;
VKSwapchain      swapchain;
VkCommandPool    commandPool;

}

std::string VKRenderer::getDeviceName() const
{
	return std::string(physicalDevice.getName());
}

std::string VKRenderer::getRendererName() const
{
	const auto version = physicalDevice.getProperties().apiVersion;
	return "Vulkan " +
		std::to_string(VK_VERSION_MAJOR(version)) + '.' +
		std::to_string(VK_VERSION_MINOR(version)) + '.' +
		std::to_string(VK_VERSION_PATCH(version));
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

	/*
	vkb::Instance vkbInstance;
	{
		vkb::InstanceBuilder builder;
		const auto result = builder.set_engine_name("graphics")
			.request_validation_layers()
			// .use_default_debug_messenger()
			.build();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbInstance = result.value();
	}
	instance = VkInstance(vkbInstance);

	glfwCreateWindowSurface(instance, reinterpret_cast<GLFWwindow*>(win.getNativeHandle()), nullptr, &surface);

	vkb::PhysicalDevice vkbPhysicalDevice;
	{
		vkb::PhysicalDeviceSelector selector(vkbInstance);
		const auto result = selector.set_surface(surface)
			.require_dedicated_transfer_queue()
			.select();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbPhysicalDevice = result.value();
	}
	physicalDevice = vkbPhysicalDevice;
	physicalDeviceProperties = vkbPhysicalDevice.properties;

	vkb::Device vkbDevice;
	{
		vkb::DeviceBuilder builder(vkbPhysicalDevice);
		const auto result = builder.build();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbDevice = result.value();
	}
	device = vkbDevice;

	{
		const auto result = vkbDevice.get_queue(vkb::QueueType::graphics);
		if(!result)
			throw std::runtime_error(result.error().message());
		queue = result.value();
	}

	{
		const auto result = vkbDevice.get_queue_index(vkb::QueueType::graphics);
		if(!result)
			throw std::runtime_error(result.error().message());
		queueIndex = result.value();
	}

	vkb::Swapchain vkbSwapchain;
	{
		vkb::SwapchainBuilder builder(vkbDevice);
		const auto result = builder.build();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbSwapchain = result.value();
	}
	swapchain = vkbSwapchain;
	*/
}

void VKRenderer::deinit()
{
	vkDestroyCommandPool(device, commandPool, nullptr);
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroyDevice(device, nullptr);
	vkDestroySurfaceKHR(instance, surface, nullptr);
	vkDestroyInstance(instance, nullptr);
}

void VKRenderer::createInstance()
{
	InstanceBuilder builder;
	instance = builder.enableValidationLayers()
		.enableDebugMessenger()
		.build();
}

void VKRenderer::createSurface(const Window& win)
{
	if(glfwCreateWindowSurface(instance, reinterpret_cast<GLFWwindow*>(win.getNativeHandle()), nullptr, &surface) != VK_SUCCESS)
		throw std::runtime_error("failed to create window surface");
}

void VKRenderer::selectPhysicalDevice()
{
	PhysicalDeviceSelector selector(instance, surface);
	physicalDevice = selector.requireGraphicsQueue()
		.requirePresentQueue()
		.requireTransferQueue()
		.select();
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
	commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolInfo.queueFamilyIndex = device.getQueueIndex(VKDevice::QueueType::Graphics);;
	commandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	if(vkCreateCommandPool(device, &commandPoolInfo, nullptr, &commandPool) != VK_SUCCESS)
		throw std::runtime_error("failed to create command pool");
}
