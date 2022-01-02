// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Renderer.h"
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
	const VkInstance& getInstance() const;

	/**
	 * @brief 获取 VkDevice.
	 */
	const VkDevice& getDevice() const;

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
};
