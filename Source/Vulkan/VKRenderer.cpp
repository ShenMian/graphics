// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKRenderer.h"
#include <VkBootstrap.h>

VkInstance       VKRenderer::instance;
VkPhysicalDevice VKRenderer::physicalDevice;
VkDevice         VKRenderer::device;
VkQueue          VKRenderer::queue;

std::string VKRenderer::getDeviceName() const
{
	return std::string();
}

std::string VKRenderer::getRendererName() const
{
	return std::string();
}

std::string VKRenderer::getVendorName() const
{
	return std::string();
}

void VKRenderer::init()
{
	vkb::Instance vkbInstance;
	{
		vkb::InstanceBuilder builder;
		const auto result = builder.request_validation_layers()
			.use_default_debug_messenger()
			.build();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbInstance = result.value();
	}
	instance = vkbInstance;

	vkb::PhysicalDevice vkbPhysicalDevice;
	{
		vkb::PhysicalDeviceSelector selector(vkbInstance);
		const auto result = selector.require_dedicated_transfer_queue()
			.select();
		if(!result)
			throw std::runtime_error(result.error().message());
		vkbPhysicalDevice = result.value();
	}
	physicalDevice = vkbPhysicalDevice;

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
