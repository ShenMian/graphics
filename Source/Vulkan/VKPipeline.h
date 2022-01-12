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

private:
	void createLayout(const PipelineLayout& layout);
	void createViewportState(VkPipelineViewportStateCreateInfo& info);
	void createRasterizerState(VkPipelineRasterizationStateCreateInfo&);
	void createMultisampleState(VkPipelineMultisampleStateCreateInfo& info);

	VkPipeline       pipeline;
	VkPipelineLayout pipelineLayout;
};
