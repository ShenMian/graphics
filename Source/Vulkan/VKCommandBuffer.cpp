// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKCommandBuffer.h"
#include "VKPipeline.h"
#include "VKVertexBuffer.h"
#include "../CommandBuffer.h"
#include "../Viewport.hpp"
#include "VKRenderer.h"
#include <array>

VKCommandBuffer::VKCommandBuffer()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& swapchain = renderer->getSwapchain();

	handles.resize(swapchain.getImages().size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = renderer->getCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = static_cast<uint32_t>(handles.size());
	if(vkAllocateCommandBuffers(renderer->getDevice(), &allocInfo, handles.data()) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffers");
}

VKCommandBuffer::~VKCommandBuffer()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	// vkFreeCommandBuffers(renderer->getDevice(), renderer->getCommandPool(), static_cast<uint32_t>(handles.size()), handles.data());
}

void VKCommandBuffer::begin()
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if(vkBeginCommandBuffer(handles[0], &beginInfo) != VK_SUCCESS)
		throw std::runtime_error("failed to begin command buffer");
}

void VKCommandBuffer::end()
{
	if(vkEndCommandBuffer(handles[0]) != VK_SUCCESS)
		throw std::runtime_error("failed to end command buffer");
}

void VKCommandBuffer::beginRenderPass()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	auto& swapchain = renderer->getSwapchain();

	std::array<VkClearValue, 2> clearValues = {};
	// clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
	clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
	clearValues[1].depthStencil = {1.0f, 0};

	VkRenderPassBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	beginInfo.renderPass = swapchain.getRenderPass();
	beginInfo.framebuffer = swapchain.getFramebuffers()[0];
	beginInfo.renderArea.offset = {0, 0};
	beginInfo.renderArea.extent = swapchain.getSize();
	beginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
	beginInfo.pClearValues = clearValues.data();
	vkCmdBeginRenderPass(handles[0], &beginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VKCommandBuffer::endRenderPass()
{
	vkCmdEndRenderPass(handles[0]);
}

void VKCommandBuffer::setViewport(const Viewport& viewport)
{
	VkViewport vkViewport;
	vkViewport.x = viewport.x;
	vkViewport.y = viewport.y;
	vkViewport.width = viewport.width;
	vkViewport.height = viewport.height;
	vkViewport.minDepth = viewport.minDepth;
	vkViewport.maxDepth = viewport.maxDepth;
	vkCmdSetViewport(handles[0], 0, 1, &vkViewport);
}

void VKCommandBuffer::setPipeline(std::shared_ptr<Pipeline> pipeline)
{
	auto vkPipeline = std::dynamic_pointer_cast<VKPipeline>(pipeline);
	vkCmdBindPipeline(handles[0], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->getNativeHandle());
}

void VKCommandBuffer::setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
	auto vkBuffer = std::dynamic_pointer_cast<VKVertexBuffer>(vertexBuffer);
	VkBuffer buffers = {*vkBuffer};
	VkDeviceSize offset = 0;
	vkCmdBindVertexBuffers(handles[0], 0, 1, &buffers, &offset);
}

void VKCommandBuffer::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
	/*
	auto vkBuffer = std::dynamic_pointer_cast<VKIndexBuffer>(indexBuffer);
	vkCmdBindIndexBuffer(handles[0], *vkBuffer, 0, VK_INDEX_TYPE_UINT32);
	*/
}

void VKCommandBuffer::clear(uint8_t flags)
{
	// vkCmdClearColorImage(handles[0], );
}

void VKCommandBuffer::setClearColor(const Vector4& color)
{
}

void VKCommandBuffer::setClearDepth(float depth)
{
}

void VKCommandBuffer::setClearStencil()
{
}

void VKCommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex)
{
	vkCmdDraw(handles[0], vertexCount, 1, firstVertex, 0);
}

void VKCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex)
{
	vkCmdDrawIndexed(handles[0], indexCount, 1, firstIndex, 0, 0);
}

VKCommandBuffer::operator VkCommandBuffer() const
{
	return handles[0];
}
