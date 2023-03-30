// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Buffer.h"
#include <cassert>
#include <cstring>

Buffer::Buffer(size_t size, Type type, Usage usage) : size_(size), type_(type), usage_(usage)
{
}

size_t Buffer::get_size() const
{
	return size_;
}

Buffer::Type Buffer::get_type() const
{
	return type_;
}

Buffer::Usage Buffer::get_usage() const
{
	return usage_;
}

void* Buffer::get_data()
{
	return data_;
}

void Buffer::write(const void* data, size_t size, size_t offset)
{
	// TODO: 添加对 mapped buffer 范围的检查
	assert(this->data_);
	assert(offset + size <= this->size_);
	std::memcpy(static_cast<unsigned char*>(this->data_) + offset, data, size);
}

void Buffer::read(void* data, size_t size, size_t offset)
{
	// TODO: 添加对 mapped buffer 范围的检查
	assert(this->data_);
	assert(offset + size <= this->size_);
	std::memcpy(data, static_cast<unsigned char*>(this->data_) + offset, size);
}
