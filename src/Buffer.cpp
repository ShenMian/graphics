// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Buffer.h"
#include <cassert>
#include <cstring>

Buffer::Buffer(size_t size, Type type, Usage usage) : size(size), type(type), usage(usage)
{
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

void* Buffer::getData()
{
	return data;
}

void Buffer::write(const void* data, size_t size, size_t offset)
{
	// TODO: 添加对 mapped buffer 范围的检查
	assert(this->data);
	assert(offset + size <= this->size);
	std::memcpy(static_cast<unsigned char*>(this->data) + offset, data, size);
}

void Buffer::read(void* data, size_t size, size_t offset)
{
	// TODO: 添加对 mapped buffer 范围的检查
	assert(this->data);
	assert(offset + size <= this->size);
	std::memcpy(data, static_cast<unsigned char*>(this->data) + offset, size);
}
