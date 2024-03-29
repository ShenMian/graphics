// Copyright 2022 ShenMian
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

std::unordered_map<CompareOp, VkCompareOp> GLCompareOp = {{CompareOp::AlwaysPass, VK_COMPARE_OP_ALWAYS},
                                                          {CompareOp::NeverPass, VK_COMPARE_OP_NEVER},
                                                          {CompareOp::Less, VK_COMPARE_OP_LESS},
                                                          {CompareOp::Equal, VK_COMPARE_OP_EQUAL},
                                                          {CompareOp::LessEqual, VK_COMPARE_OP_LESS_OR_EQUAL},
                                                          {CompareOp::Greater, VK_COMPARE_OP_GREATER},
                                                          {CompareOp::GreaterEqual, VK_COMPARE_OP_GREATER_OR_EQUAL},
                                                          {CompareOp::NotEqual, VK_COMPARE_OP_NOT_EQUAL}};

std::unordered_map<Format, VkFormat> VKFormat = {
    {Format::R16F, VK_FORMAT_R16_SFLOAT},         {Format::RG16F, VK_FORMAT_R16G16_SFLOAT},
    {Format::RGB16F, VK_FORMAT_R16G16B16_SFLOAT}, {Format::RGBA16F, VK_FORMAT_R16G16B16A16_SFLOAT},
    {Format::R32F, VK_FORMAT_R32_SFLOAT},         {Format::RG32F, VK_FORMAT_R32G32_SFLOAT},
    {Format::RGB32F, VK_FORMAT_R32G32B32_SFLOAT}, {Format::RGBA32F, VK_FORMAT_R32G32B32A32_SFLOAT},
};

std::unordered_map<PipelineLayout::Type, VkDescriptorType> VKType = {
    {PipelineLayout::Type::UniformBuffer, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER},
    {PipelineLayout::Type::Sampler, VK_DESCRIPTOR_TYPE_SAMPLER},
    {PipelineLayout::Type::Texture, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE},
};

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
	auto& swapchain = renderer->get_swapchain();

	auto program = std::dynamic_pointer_cast<VKProgram>(desc.program);

	create_pipeline_layout(desc);

	std::vector<VkVertexInputBindingDescription>   bindings;
	std::vector<VkVertexInputAttributeDescription> attribs;
	VkPipelineVertexInputStateCreateInfo           vertexInputState = {};
	create_vertex_input_state(vertexInputState, desc, bindings, attribs);

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	create_input_assembly_state(inputAssemblyState, desc);

	std::vector<VkViewport>           vkViewports;
	std::vector<VkRect2D>             vkScissors;
	VkPipelineViewportStateCreateInfo viewportState = {};
	create_viewport_state(viewportState, desc, vkViewports, vkScissors);

	VkPipelineRasterizationStateCreateInfo rasterizerState = {};
	create_rasterizer_state(rasterizerState, desc);

	VkPipelineMultisampleStateCreateInfo multisampleState = {};
	create_multisample_state(multisampleState, desc);

	VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
	create_depth_stencil_state(depthStencilState, desc);

	std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments = {};
	VkPipelineColorBlendStateCreateInfo              colorBlendState       = {};
	create_color_blend_state(colorBlendState, desc, colorBlendAttachments);

	VkPipelineDynamicStateCreateInfo dynamicState = {};
	std::vector<VkDynamicState>      dynamicStates;
	create_dynamic_state(dynamicState, dynamicStates);

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType                        = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount                   = program->get_stage_count();
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
	pipelineInfo.layout           = pipeline_layout_;
	pipelineInfo.renderPass       = swapchain.get_render_pass();
	if(vkCreateGraphicsPipelines(renderer->get_device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_) !=
	   VK_SUCCESS)
		throw std::runtime_error("failed to create graphics pipeline");
}

void VKPipeline::bind()
{
}

VKPipeline::operator VkPipeline() noexcept
{
	return pipeline_;
}

VKPipeline::operator VkPipeline() const noexcept
{
	return pipeline_;
}

void VKPipeline::create_pipeline_layout(const Descriptor& desc)
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());

	std::vector<VkDescriptorSetLayoutBinding> bindings;
	for(const auto& binding : desc.layout.get_bindings())
	{
		VkDescriptorSetLayoutBinding layoutBinding = {};
		layoutBinding.binding                      = binding.binding;
		layoutBinding.descriptorType               = VKType[binding.type];
		layoutBinding.descriptorCount              = binding.arraySize;
		layoutBinding.stageFlags                   = VKStageFlags(binding.stageFlags);
		bindings.emplace_back(layoutBinding);
	}

	VkDescriptorSetLayout           setLayout;
	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType                           = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount                    = static_cast<uint32_t>(bindings.size());
	layoutInfo.pBindings                       = bindings.data();
	if(vkCreateDescriptorSetLayout(renderer->get_device(), &layoutInfo, nullptr, &setLayout) != VK_SUCCESS)
		throw std::runtime_error("failed to create descriptor set layout");

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType                      = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount             = 1;
	pipelineLayoutInfo.pSetLayouts                = &setLayout;
	if(vkCreatePipelineLayout(renderer->get_device(), &pipelineLayoutInfo, nullptr, &pipeline_layout_) != VK_SUCCESS)
		throw std::runtime_error("failed to create pipeline layout");
}

void VKPipeline::create_vertex_input_state(VkPipelineVertexInputStateCreateInfo& info, const Descriptor& desc,
                                        std::vector<VkVertexInputBindingDescription>&   bindings,
                                        std::vector<VkVertexInputAttributeDescription>& attribs)
{
	const auto& layout = desc.vertexFormat;

	VkVertexInputBindingDescription binding = {};
	binding.binding                         = 0;
	binding.inputRate                       = VK_VERTEX_INPUT_RATE_VERTEX;
	binding.stride                          = layout.get_stride();
	bindings.push_back(binding);

	for(const auto& attr : layout.get_attributes())
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

void VKPipeline::create_input_assembly_state(VkPipelineInputAssemblyStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	info.primitiveRestartEnable = VK_FALSE;
}

void VKPipeline::create_viewport_state(VkPipelineViewportStateCreateInfo& info, const Descriptor& desc,
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

void VKPipeline::create_rasterizer_state(VkPipelineRasterizationStateCreateInfo& info, const Descriptor& desc)
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
	info.lineWidth               = desc.rasterizer.lineWidth;
}

void VKPipeline::create_multisample_state(VkPipelineMultisampleStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	info.sampleShadingEnable   = VK_FALSE;
	info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
	info.minSampleShading      = 1.0f;     // Optional
	info.pSampleMask           = nullptr;  // Optional
	info.alphaToCoverageEnable = VK_FALSE; // Optional
	info.alphaToOneEnable      = VK_FALSE; // Optional
}

void VKPipeline::create_depth_stencil_state(VkPipelineDepthStencilStateCreateInfo& info, const Descriptor& desc)
{
	info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	info.depthTestEnable       = desc.depth.enableTest;
	info.depthWriteEnable      = desc.depth.enableWrite;
	info.depthCompareOp        = GLCompareOp[desc.depth.compareOp];
	info.depthBoundsTestEnable = VK_FALSE;
	info.stencilTestEnable     = VK_FALSE;
	info.front                 = {};   // Optional
	info.back                  = {};   // Optional
	info.minDepthBounds        = 0.0f; // Optional
	info.maxDepthBounds        = 1.0f; // Optional
}

void VKPipeline::create_color_blend_state(VkPipelineColorBlendStateCreateInfo& info, const Descriptor& desc,
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

void VKPipeline::create_dynamic_state(VkPipelineDynamicStateCreateInfo& info, std::vector<VkDynamicState>& dynamicStates)
{
	dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
	dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

	info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	info.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	info.pDynamicStates    = dynamicStates.data();
}
