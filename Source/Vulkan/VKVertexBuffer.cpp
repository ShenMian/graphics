// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKVertexBuffer.h"
#include "VKRenderer.h"

VKVertexBuffer::VKVertexBuffer(const void* data, size_t size, uint32_t count, const VertexLayout& layout, Usage usage)
	: VertexBuffer(size, count, layout)
{
}

VKVertexBuffer::~VKVertexBuffer()
{
}

void VKVertexBuffer::bind()
{
}
