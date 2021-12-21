﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Renderer.h"
#include <Vulkan/vulkan.h>

class VKRenderer : public Renderer
{
public:
	std::string getDeviceName() const override;
	std::string getRendererName() const override;
	std::string getVendorName() const override;

	static void init();

private:
	static VkInstance       instance;
	static VkPhysicalDevice physicalDevice;
	static VkDevice         device;
	static VkQueue          queue;
};
