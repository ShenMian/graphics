// Copyright 2022 ShenMian
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

	operator VkPipeline() noexcept;
	operator VkPipeline() const noexcept;

private:
	void create_pipeline_layout(const Descriptor& desc);
	void create_vertex_input_state(VkPipelineVertexInputStateCreateInfo& info, const Descriptor& desc,
	                            std::vector<VkVertexInputBindingDescription>&   bindings,
	                            std::vector<VkVertexInputAttributeDescription>& attribs);
	void create_input_assembly_state(VkPipelineInputAssemblyStateCreateInfo&, const Descriptor& desc);
	void create_viewport_state(VkPipelineViewportStateCreateInfo& info, const Descriptor& desc,
	                         std::vector<VkViewport>& viewports, std::vector<VkRect2D>& scissors);
	void create_rasterizer_state(VkPipelineRasterizationStateCreateInfo& info, const Descriptor& desc);
	void create_multisample_state(VkPipelineMultisampleStateCreateInfo& info, const Descriptor& desc);
	void create_depth_stencil_state(VkPipelineDepthStencilStateCreateInfo& info, const Descriptor& desc);
	void create_color_blend_state(VkPipelineColorBlendStateCreateInfo& info, const Descriptor& desc,
	                           std::vector<VkPipelineColorBlendAttachmentState>& colorBlendAttachments);
	void create_dynamic_state(VkPipelineDynamicStateCreateInfo& info, std::vector<VkDynamicState>& dynamicStates);

	VkPipeline       pipeline_;
	VkPipelineLayout pipeline_layout_;
};
