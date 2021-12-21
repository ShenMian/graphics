// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKRenderer.h"
#include <VkBootstrap.h>

#include <GLFW/glfw3.h>

namespace
{

VkInstance                 instance;
VkPhysicalDevice           physicalDevice;
VkPhysicalDeviceProperties physicalDeviceProperties;
VkDevice                   device;
VkQueue                    queue;

}

std::string VKRenderer::getDeviceName() const
{
	return physicalDeviceProperties.deviceName;
}

std::string VKRenderer::getRendererName() const
{
	const auto version = physicalDeviceProperties.apiVersion;
	return "Vulkan " +
		std::to_string(VK_VERSION_MAJOR(version)) + '.' +
		std::to_string(VK_VERSION_MINOR(version)) + '.' +
		std::to_string(VK_VERSION_PATCH(version));
}

std::string VKRenderer::getVendorName() const
{
	const auto id = physicalDeviceProperties.vendorID;
	switch(id)
	{
	case 0x1002: return "Advanced Micro Devices, Inc.";
	case 0x10de: return "NVIDIA Corporation";
	case 0x102b: return "Matrox Electronic Systems Ltd.";
	case 0x1414: return "Microsoft Corporation";
	case 0x5333: return "S3 Graphics Co., Ltd.";
	case 0x8086: return "Intel Corporation";
	case 0x80ee: return "Oracle Corporation";
	case 0x15ad: return "VMware Inc.";
	}
	return "Unknown";
}

void VKRenderer::init(const Window& win)
{
	vkb::Instance vkbInstance;
	{
		vkb::InstanceBuilder builder;
		const auto result = builder.request_validation_layers()
			// .use_default_debug_messenger()
			.build();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbInstance = result.value();
	}
	instance = vkbInstance;

	vkb::PhysicalDevice vkbPhysicalDevice;
	{
		VkSurfaceKHR surface;
		glfwCreateWindowSurface(instance, reinterpret_cast<GLFWwindow*>(win.getNativeHandle()), nullptr, &surface);

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
}
