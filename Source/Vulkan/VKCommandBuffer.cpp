// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "VKCommandBuffer.h"
#include "../CommandBuffer.h"
#include "VKRenderer.h"

VKCommandBuffer::VKCommandBuffer()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());

	VkCommandBufferAllocateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	info.pNext = NULL;
	info.commandPool = renderer->getCommandPool();
	info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	info.commandBufferCount = 1;
	const auto ret = vkAllocateCommandBuffers(renderer->getDevice(), &info, &handle);
	if(ret != VK_SUCCESS)
		throw std::runtime_error("failed to create command buffer");
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
	vkBeginCommandBuffer(handle, &info);
}

void VKCommandBuffer::end()
{
	vkEndCommandBuffer(handle);
}

void VKCommandBuffer::setViewport(const Vector2i& origin, const Vector2i& size)
{
}

void VKCommandBuffer::setPipeline(std::shared_ptr<Pipeline> pipeline)
{
}

void VKCommandBuffer::setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
}

void VKCommandBuffer::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
}

void VKCommandBuffer::clear(uint8_t flags)
{
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

void VKCommandBuffer::draw(size_t firstVertex, size_t verticesNum)
{
}

void VKCommandBuffer::drawIndexed(size_t firstIndex, size_t indicesNum)
{
}
