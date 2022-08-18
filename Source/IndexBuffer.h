// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include <cstdint>
#include <memory>
#include <span>
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
	[[nodiscard]] static std::shared_ptr<IndexBuffer> create(std::span<const uint32_t> data,
	                                                         Buffer::Usage             usage = Buffer::Usage::Static);

	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  索引缓冲区指针.
	 * @param size  索引缓冲区大小, 单位: 字节.
	 * @param count 索引数量.
	 * @param usage 使用方式.
	 */
	[[nodiscard]] static std::shared_ptr<IndexBuffer> create(const uint32_t* data, size_t size,
	                                                         Buffer::Usage usage = Buffer::Usage::Static);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	[[nodiscard]] size_t getSize() const;

	/**
	 * @brief 获取索引数.
	 */
	[[nodiscard]] uint32_t getCount() const;

	virtual Buffer& getBuffer() = 0;

protected:
	IndexBuffer(const void* data, size_t size);

private:
	size_t   size;
	uint32_t count;
};
