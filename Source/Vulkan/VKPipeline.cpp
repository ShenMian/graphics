// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKPipeline.h"
#include "../PipelineLayout.h"
#include "VKProgram.h"
#include "VKVertexBuffer.h"
#include <stdexcept>
#include <unordered_map>

namespace
{

std::unordered_map<PolygonMode, VkPolygonMode> VKPolygonMode = {{PolygonMode::Fill, VK_POLYGON_MODE_FILL},
                                                                {PolygonMode::Wireframe, VK_POLYGON_MODE_LINE}};

std::unordered_map<CullMode, VkCullModeFlags> VKCullMode = {
    {CullMode::Disabled, VK_CULL_MODE_NONE},
    {CullMode::Front, VK_CULL_MODE_FRONT_BIT},
    {CullMode::Back, VK_CULL_MODE_BACK_BIT},
};

std::unordered_map<Format, VkFormat> VKFormat = {
    {Format::R16F, VK_FORMAT_R16_SFLOAT},         {Format::RG16F, VK_FORMAT_R16G16_SFLOAT},
    {Format::RGB16F, VK_FORMAT_R16G16B16_SFLOAT}, {Format::RGBA16F, VK_FORMAT_R16G16B16A16_SFLOAT},
    {Format::R32F, VK_FORMAT_R32_SFLOAT},         {Format::RG32F, VK_FORMAT_R32G32_SFLOAT},
    {Format::RGB32F, VK_FORMAT_R32G32B32_SFLOAT}, {Format::RGBA32F, VK_FORMAT_R32G32B32A32_SFLOAT},
};

VkDescriptorType VKType(PipelineLayout::Type type)
{
	switch(type)
	{
		using enum PipelineLayout::Type;

	case UniformBuffer:
		return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	case Sampler:
		return VK_DESCRIPTOR_TYPE_SAMPLER;
	case Texture:
		return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
	};
	throw std::invalid_argument("");
}

VkShaderStageFlags VKStageFlags(uint32_t flags)
{
	VkShaderStageFlags vkFlags = 0;
	if(flags & PipelineLayout::StageFlags::Vertex)
		vkFlags |= VK_SHADER_STAGE_VERTEX_BIT;
	if(flags & PipelineLayout::StageFlags::Geometry)
		vkFlags |= VK_SHADER_STAGE_GEOMETRY_BIT;
	if(flags & PipelineLayout::StageFlags::Fragment)
		vkFlags |= VK_SHADER_STAGE_FRAGMENT_BIT;
	if(flags & PipelineLayout::StageFlags::Compute)
		vkFlags |= VK_SHADER_STAGE_COMPUTE_BIT;
	return vkFlags;
}

} // namespace

VKPipeline::VKPipeline(const Descriptor& desc) : Pipeline(desc)
{
	auto  renderer  = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& swapchain = renderer->getSwapchain();

	auto program = std::dynamic_pointer_cast<VKProgram>(desc.program);

	createPipelineLayout(desc);

	std::vector<VkVertexInputBindingDescription>   bindings;
	std::vector<VkVertexInputAttributeDescription> attribs;
	VkPipelineVertexInputStateCreateInfo           vertexInputState = {};
	createVertexInputState(vertexInputState, desc, bindings, attribs);

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	createInputAssemblyState(inputAssemblyState, desc);

	std::vector<VkViewport>           vkViewports;
	std::vector<VkRect2D>             vkScissors;
	VkPipelineViewportStateCreateInfo viewportState = {};
	createViewportState(viewportState, desc, vkViewports, vkScissors);

	VkPipelineRasterizationStateCreateInfo rasterizerState = {};
	createRasterizerState(rasterizerState, desc);

	VkPipelineMultisampleStateCreateInfo multisampleState = {};
	createMultisampleState(multisampleState, desc);

	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	createDepthStencilState(depthStencilState, desc);

	std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments = {};
	VkPipelineColorBlendStateCreateInfo              colorBlendState       = {};
	createColorBlendState(colorBlendState, desc, colorBlendAttachments);

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	std::vector<VkDynamicState>      dynamicStates;
	createDynamicState(dynamicState, dynamicStates);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount                   = program->getStageCount();
	pipelineInfo.pStages                      = program->getInfos().data();
	pipelineInfo.pVertexInputState            = &vertexInputState;
	pipelineInfo.pInputAssemblyState          = &inputAssemblyState;
	// pipelineInfo.pTessellationState = (inputAssembly.topology == VK_PRIMITIVE_TOPOLOGY_PATCH_LIST ?
	// &tessellationState : nullptr);
	pipelineInfo.pViewportState      = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizerState;
	pipelineInfo.pMultisampleState   = &multisampleState;
	// pipelineInfo.pDepthStencilState = &depthStencilState;
	pipelineInfo.pColorBlendState = &colorBlendState;
	pipelineInfo.pDynamicState    = &dynamicState;
	pipelineInfo.layout           = pipelineLayout;
	pipelineInfo.renderPass       = swapchain.getRenderPass();
	if(vkCreateGraphicsPipelines(renderer->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) !=
	   VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline");
}

void VKPipeline::bind()
{
}

VkPipeline VKPipeline::getHandle()
{
	return pipeline;
}

void VKPipeline::createPipelineLayout(const Descriptor& desc)
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());

	std::vector<VkDescriptorSetLayoutBinding> bindings;

	for(const auto& binding : desc.layout.getBindings())
	{
		VkDescriptorSetLayoutBinding layoutBinding = {};
		layoutBinding.binding                      = binding.binding;
		layoutBinding.descriptorType               = VKType(binding.type);
		layoutBinding.descriptorCount              = binding.arraySize;
		layoutBinding.stageFlags                   = VKStageFlags(binding.stageFlags);
		bindings.emplace_back(layoutBinding);
	}

	VkDescriptorSetLayout           setLayout;
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount                    = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings                       = bindings.data();
	if(vkCreateDescriptorSetLayout(renderer->getDevice(), &layoutInfo, nullptr, &setLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor set layout");

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount             = 1;
	pipelineLayoutInfo.pSetLayouts                = &setLayout;
	if(vkCreatePipelineLayout(renderer->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}

void VKPipeline::createVertexInputState(VkPipelineVertexInputStateCreateInfo& info, const Descriptor& desc,
                                        std::vector<VkVertexInputBindingDescription>&   bindings,
                                        std::vector<VkVertexInputAttributeDescription>& attribs)
{
	const auto& layout = desc.vertexFormat;

	VkVertexInputBindingDescription binding = {};
	binding.binding                         = 0;
	binding.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;
	binding.stride                          = layout.getStride();
	bindings.push_back(binding);

	for(const auto& attr : layout.getAttributes())
	{
		VkVertexInputAttributeDescription vkAttr = {};
		vkAttr.location                          = attr.location;
		vkAttr.format                            = VKFormat[attr.format];
		vkAttr.offset                            = attr.offset;
		attribs.push_back(vkAttr);
	}

	info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	info.vertexBindingDescriptionCount   = static_cast<uint32_t>(bindings.size());
	info.pVertexBindingDescriptions      = bindings.data();
	info.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribs.size());
	info.pVertexAttributeDescriptions    = attribs.data();
}

void VKPipeline::createInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	info.primitiveRestartEnable = VK_FALSE;
}

void VKPipeline::createViewportState(VkPipelineViewportStateCreateInfo& info, const Descriptor& desc,
                                     std::vector<VkViewport>& vkViewports, std::vector<VkRect2D>& vkScissors)
{
	for(const auto& viewport : desc.viewports)
	{
		VkViewport vkViewport;
		vkViewport.x        = viewport.x;
		vkViewport.y        = viewport.y + viewport.height;
		vkViewport.width    = viewport.width;
		vkViewport.height   = -viewport.height;
		vkViewport.minDepth = viewport.minDepth;
		vkViewport.maxDepth = viewport.maxDepth;
		vkViewports.push_back(vkViewport);

		VkRect2D scissor;
		scissor.offset = {static_cast<int>(viewport.x), static_cast<int>(viewport.y)};
		scissor.extent = {static_cast<unsigned int>(viewport.width), static_cast<unsigned int>(viewport.height)};
		vkScissors.push_back(scissor);
	}

	info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	info.viewportCount = static_cast<uint32_t>(vkViewports.size());
	info.pViewports    = vkViewports.data();
	info.scissorCount  = static_cast<uint32_t>(vkScissors.size());
	info.pScissors     = vkScissors.data();
}

void VKPipeline::createRasterizerState(VkPipelineRasterizationStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	info.depthClampEnable        = VK_FALSE;
	info.rasterizerDiscardEnable = VK_FALSE;
	info.polygonMode             = VKPolygonMode[desc.rasterizer.polygonMode];
	info.cullMode                = VKCullMode[desc.rasterizer.cullMode];
	info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
	info.depthBiasEnable         = VK_FALSE;
	info.depthBiasConstantFactor = 0.0f; // Optional
	info.depthBiasClamp          = 0.0f; // Optional
	info.depthBiasSlopeFactor    = 0.0f; // Optional
	info.lineWidth               = 1.0f;
}

void VKPipeline::createMultisampleState(VkPipelineMultisampleStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.sampleShadingEnable   = VK_FALSE;
	info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
	info.minSampleShading      = 1.0f;     // Optional
	info.pSampleMask           = nullptr;  // Optional
	info.alphaToCoverageEnable = VK_FALSE; // Optional
	info.alphaToOneEnable      = VK_FALSE; // Optional
}

void VKPipeline::createDepthStencilState(VkPipelineDepthStencilStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.depthTestEnable       = VK_TRUE;
	info.depthWriteEnable      = VK_TRUE;
	info.depthCompareOp        = VK_COMPARE_OP_LESS;
	info.depthBoundsTestEnable = VK_FALSE;
	info.stencilTestEnable     = VK_FALSE;
	info.front                 = {};   // Optional
	info.back                  = {};   // Optional
	info.minDepthBounds        = 0.0f; // Optional
	info.maxDepthBounds        = 1.0f; // Optional
}

void VKPipeline::createColorBlendState(VkPipelineColorBlendStateCreateInfo& info, const Descriptor& desc,
                                       std::vector<VkPipelineColorBlendAttachmentState>& attachments)
{
	VkPipelineColorBlendAttachmentState attachement = {};
	attachement.blendEnable                         = VK_FALSE;
	attachement.srcColorBlendFactor                 = VK_BLEND_FACTOR_ONE;  // Optional
	attachement.dstColorBlendFactor                 = VK_BLEND_FACTOR_ZERO; // Optional
	attachement.colorBlendOp                        = VK_BLEND_OP_ADD;      // Optional
	attachement.srcAlphaBlendFactor                 = VK_BLEND_FACTOR_ONE;  // Optional
	attachement.dstAlphaBlendFactor                 = VK_BLEND_FACTOR_ZERO; // Optional
	attachement.alphaBlendOp                        = VK_BLEND_OP_ADD;      // Optional
	attachement.colorWriteMask =
	    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	attachments.push_back(attachement);

	info.sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	info.logicOpEnable     = VK_FALSE;
	info.logicOp           = VK_LOGIC_OP_COPY; // Optional
	info.attachmentCount   = static_cast<uint32_t>(attachments.size());
	info.pAttachments      = attachments.data();
	info.blendConstants[0] = 0.0f; // Optional
	info.blendConstants[1] = 0.0f; // Optional
	info.blendConstants[2] = 0.0f; // Optional
	info.blendConstants[3] = 0.0f; // Optional
}

void VKPipeline::createDynamicState(VkPipelineDynamicStateCreateInfo& info, std::vector<VkDynamicState>& dynamicStates)
{
	dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

	info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	info.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	info.pDynamicStates    = dynamicStates.data();
}
