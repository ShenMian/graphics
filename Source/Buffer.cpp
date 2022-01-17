// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Buffer.h"

Buffer::Buffer(size_t size, Type type, Usage usage)
	: size(size), type(type), usage(usage)
{
}

void* Buffer::getData()
{
	return data;
}

size_t Buffer::getSize() const
{
	return size;
}

Buffer::Type Buffer::getType() const
{
	return type;
}

Buffer::Usage Buffer::getUsage() const
{
	return usage;
}
