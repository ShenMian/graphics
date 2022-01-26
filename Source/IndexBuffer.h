// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include <cstdint>
#include <memory>
#include <vector>

/**
 * @brief 索引缓冲区.
 */
class IndexBuffer
{
public:
	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  索引缓冲区.
	 * @param usage 使用方式.
	 */
	static std::shared_ptr<IndexBuffer> create(const std::vector<uint32_t>& data, Buffer::Usage usage = Buffer::Usage::Static);

	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  索引缓冲区指针.
	 * @param size  索引缓冲区大小, 单位: 字节.
	 * @param count 索引数量.
	 * @param usage 使用方式.
	 */
	static std::shared_ptr<IndexBuffer> create(const uint32_t* data, size_t size, Buffer::Usage usage = Buffer::Usage::Static);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	size_t getSize() const;

	/**
	 * @brief 获取索引数.
	 */
	uint32_t getCount() const;

	virtual void map() = 0;
	virtual void unmap() = 0;
	virtual void write(const void* data, size_t size) = 0;

	virtual void bind() = 0;

protected:
	IndexBuffer(const void* data, size_t size);

private:
	size_t   size;
	uint32_t count;
};
