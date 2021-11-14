// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "VKCommandBuffer.h"
#include "../CommandBuffer.h"

void VKCommandBuffer::begin()
{
}

void VKCommandBuffer::end()
{
}

void VKCommandBuffer::setViewport(const Vector2i& origin, const Vector2i& size)
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

void VKCommandBuffer::draw(size_t first, size_t num)
{
}
