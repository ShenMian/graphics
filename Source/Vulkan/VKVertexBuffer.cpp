// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKVertexBuffer.h"
#include "VKRenderer.h"

VKVertexBuffer::VKVertexBuffer(const void* data, size_t size, uint32_t count, const VertexLayout& layout, Usage usage)
	: VertexBuffer(data, size, layout)
{
}

VKVertexBuffer::~VKVertexBuffer()
{
}

void VKVertexBuffer::write(const void* data, size_t size)
{
}

void VKVertexBuffer::bind()
{
}
