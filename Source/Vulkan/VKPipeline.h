// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "VKBuffer.h"
#include "../Pipeline.h"
#include <vulkan/vulkan.h>

class VKPipeline : public Pipeline
{
public:
	VKPipeline(const Descriptor& desc);

	VkRenderPass getRendererPass();
	VkPipeline getNativeHandle();

	VkFramebuffer framebuffer;
	VkRenderPass  renderPass;

private:
	void createRenderPass();

	void createLayout(const Descriptor& desc);
	void createInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo&, const Descriptor& desc);
	void createViewportState(VkPipelineViewportStateCreateInfo& info, const Descriptor& desc, std::vector<VkViewport>& vkViewports, std::vector<VkRect2D>& vkScissors);
	void createRasterizerState(VkPipelineRasterizationStateCreateInfo& info, const Descriptor& desc);
	void createMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const Descriptor& desc);
	void createDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const Descriptor& desc);
	void createColorBlendState(VkPipelineColorBlendStateCreateInfo& info, const Descriptor& desc, std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachments);

	VkPipeline       pipeline;
	VkPipelineLayout pipelineLayout;
};
