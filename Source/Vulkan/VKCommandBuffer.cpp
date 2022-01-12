// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "VKCommandBuffer.h"
#include "../CommandBuffer.h"
#include "../Viewport.hpp"
#include "VKRenderer.h"

VKCommandBuffer::VKCommandBuffer()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());

	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.commandPool = renderer->getCommandPool();
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	info.commandBufferCount = 1;
	if(vkAllocateCommandBuffers(renderer->getDevice(), &info, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to allocate command buffer");
}

VKCommandBuffer::~VKCommandBuffer()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	vkFreeCommandBuffers(renderer->getDevice(), renderer->getCommandPool(), 1, &handle);
}

void VKCommandBuffer::begin()
{
	VkCommandBufferBeginInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	if(vkBeginCommandBuffer(handle, &info) != VK_SUCCESS)
		throw std::runtime_error("failed to begin command buffer");
}

void VKCommandBuffer::end()
{
	if(vkEndCommandBuffer(handle) != VK_SUCCESS)
		throw std::runtime_error("failed to end command buffer");
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
	vkCmdSetViewport(handle, 0, 1, &vkViewport);
}

void VKCommandBuffer::setPipeline(std::shared_ptr<Pipeline> pipeline)
{
}

void VKCommandBuffer::setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
	// vkCmdBindVertexBuffers(handle, );
}

void VKCommandBuffer::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
	// vkCmdBindIndexBuffer(handle, );
}

void VKCommandBuffer::clear(uint8_t flags)
{
	// vkCmdClearColorImage(handle, );
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
	vkCmdDraw(handle, vertexCount, 1, firstVertex, 0);
}

void VKCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex)
{
	vkCmdDrawIndexed(handle, indexCount, 1, firstIndex, 0, 0);
}
