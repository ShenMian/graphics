// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Renderer.h"
#include "VKDevice.h"
#include "VKInstance.h"
#include "VKPhysicalDevice.h"
#include <vulkan/vulkan.h>

class VKRenderer : public Renderer
{
public:
	std::string getDeviceName() const override;
	std::string getRendererName() const override;
	std::string getVendorName() const override;

	/**
	 * @brief 获取 VkInstance.
	 */
	const VKInstance& getInstance() const;

	/**
	 * @brief 获取 VkPhysicalDevice.
	 */
	const VKPhysicalDevice& getPhysicalDevice() const;

	/**
	 * @brief 获取 VkDevice.
	 */
	const VKDevice& getDevice() const;

	/**
	 * @brief 获取 VkQueue.
	 */
	const VkQueue& getQueue() const;

	/**
	 * @brief 获取命令缓冲区池.
	 */
	const VkCommandPool& getCommandPool() const;

	static void init(const Window& win);
	static void deinit();

private:
	static void createInstance();
	static void createSurface(const Window& win);
	static void selectPhysicalDevice();
	static void createDevice();
};
