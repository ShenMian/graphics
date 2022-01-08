// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKVertexBuffer.h"
#include "VKRenderer.h"

VKVertexBuffer::VKVertexBuffer(const void* data, size_t size, size_t count, const VertexFormat& fmt, Usage usage)
	: VertexBuffer(size, count, fmt)
{
}

VKVertexBuffer::~VKVertexBuffer()
{
}

void VKVertexBuffer::bind()
{
}
