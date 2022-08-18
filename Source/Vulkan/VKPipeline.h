// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Pipeline.h"
#include "VKBuffer.h"
#include <vector>
#include <vulkan/vulkan.h>

class VKPipeline : public Pipeline
{
public:
	VKPipeline(const Descriptor& desc);

	void bind() override;

	VkPipeline getNativeHandle();

private:
	void createPipelineLayout(const Descriptor& desc);
	void createVertexInputState(VkPipelineVertexInputStateCreateInfo& info, const Descriptor& desc,
	                            std::vector<VkVertexInputBindingDescription>&   bindings,
	                            std::vector<VkVertexInputAttributeDescription>& attribs);
	void createInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo&, const Descriptor& desc);
	void createViewportState(VkPipelineViewportStateCreateInfo& info, const Descriptor& desc,
	                         std::vector<VkViewport>& viewports, std::vector<VkRect2D>& scissors);
	void createRasterizerState(VkPipelineRasterizationStateCreateInfo& info, const Descriptor& desc);
	void createMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const Descriptor& desc);
	void createDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const Descriptor& desc);
	void createColorBlendState(VkPipelineColorBlendStateCreateInfo& info, const Descriptor& desc,
	                           std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachments);
	void createDynamicState(VkPipelineDynamicStateCreateInfo& info, std::vector<VkDynamicState>& dynamicStates);

	VkPipeline       pipeline;
	VkPipelineLayout pipelineLayout;
};
