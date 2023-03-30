// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "VKCommandBuffer.h"
#include "../CommandBuffer.h"
#include "../Viewport.hpp"
#include "VKIndexBuffer.h"
#include "VKPipeline.h"
#include "VKRenderer.h"
#include "VKVertexBuffer.h"
#include <array>

VKCommandBuffer::VKCommandBuffer()
{
	auto  renderer  = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& swapchain = renderer->get_swapchain();

	handles_.resize(swapchain.get_images().size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType                       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool                 = renderer->get_command_pool();
	allocInfo.level                       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount          = static_cast<uint32_t>(handles_.size());
	if(vkAllocateCommandBuffers(renderer->get_device(), &allocInfo, handles_.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers");
}

VKCommandBuffer::~VKCommandBuffer()
{
	// auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	// vkFreeCommandBuffers(renderer->getDevice(), renderer->getCommandPool(), static_cast<uint32_t>(handles.size()),
	// handles.data());
}

void VKCommandBuffer::begin()
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if(vkBeginCommandBuffer(handles_[index_], &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin command buffer");
}

void VKCommandBuffer::end()
{
	if(vkEndCommandBuffer(handles_[index_]) != VK_SUCCESS)
		throw std::runtime_error("failed to end command buffer");

	index_ = (index_ + 1) % handles_.size(); // TODO: debug
}

void VKCommandBuffer::begin_render_pass()
{
	auto  renderer  = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& swapchain = renderer->get_swapchain();

	std::array<VkClearValue, 2> clearValues = {};
	// clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
	clearValues[0].color        = {0.0f, 0.0f, 0.0f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass            = swapchain.get_render_pass();
	beginInfo.framebuffer           = swapchain.get_framebuffers()[0];
	beginInfo.renderArea.offset     = {0, 0};
	beginInfo.renderArea.extent     = swapchain.get_size();
	beginInfo.clearValueCount       = static_cast<uint32_t>(clearValues.size());
	beginInfo.pClearValues          = clearValues.data();
	vkCmdBeginRenderPass(handles_[index_], &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VKCommandBuffer::end_render_pass()
{
	vkCmdEndRenderPass(handles_[index_]);
}

void VKCommandBuffer::set_viewport(const Viewport& viewport)
{
	VkViewport vkViewport;
	vkViewport.x        = viewport.x;
	vkViewport.y        = viewport.y + viewport.height;
	vkViewport.width    = viewport.width;
	vkViewport.height   = -viewport.height;
	vkViewport.minDepth = viewport.minDepth;
	vkViewport.maxDepth = viewport.maxDepth;
	vkCmdSetViewport(handles_[index_], 0, 1, &vkViewport);
}

void VKCommandBuffer::set_pipeline(std::shared_ptr<Pipeline> p)
{
	auto pipeline = std::dynamic_pointer_cast<VKPipeline>(p);
	vkCmdBindPipeline(handles_[index_], VK_PIPELINE_BIND_POINT_GRAPHICS, *pipeline);
}

void VKCommandBuffer::set_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer)
{
	auto         buffer    = std::dynamic_pointer_cast<VKVertexBuffer>(vertex_buffer);
	VkBuffer     buffers[] = {*buffer};
	VkDeviceSize offset    = 0;
	vkCmdBindVertexBuffers(handles_[index_], 0, 1, buffers, &offset);
}

void VKCommandBuffer::set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer)
{
	auto buffer = std::dynamic_pointer_cast<VKIndexBuffer>(index_buffer);
	vkCmdBindIndexBuffer(handles_[index_], *buffer, 0, VK_INDEX_TYPE_UINT32);
}

void VKCommandBuffer::set_texture(std::shared_ptr<Texture> texture, unsigned int slot)
{
	// TODO
}

// FIXME
void VKCommandBuffer::clear(uint8_t flags)
{
	auto  renderer  = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& swapchain = renderer->get_swapchain();

	VkImageSubresourceRange imageRange = {};
	imageRange.aspectMask              = VK_IMAGE_ASPECT_COLOR_BIT;
	imageRange.levelCount              = 1;
	imageRange.layerCount              = 1;

	vkCmdClearColorImage(handles_[index_], swapchain.get_images()[index_], VK_IMAGE_LAYOUT_GENERAL, &clear_color_, 1,
	                     &imageRange);
}

void VKCommandBuffer::set_clear_color(const Vector4& color)
{
	clear_color_.float32[0] = color.x();
	clear_color_.float32[1] = color.y();
	clear_color_.float32[2] = color.z();
	clear_color_.float32[3] = color.w();
}

void VKCommandBuffer::set_clear_depth(float depth)
{
}

void VKCommandBuffer::set_clear_stencil(uint32_t value)
{
}

void VKCommandBuffer::draw(uint32_t vertex_count, uint32_t first_vertex)
{
	vkCmdDraw(handles_[index_], vertex_count, 1, first_vertex, 0);
}

void VKCommandBuffer::draw_indexed(uint32_t index_count, uint32_t first_index)
{
	vkCmdDrawIndexed(handles_[index_], index_count, 1, first_index, 0, 0);
}

VKCommandBuffer::operator VkCommandBuffer() const
{
	return handles_[index_];
}
