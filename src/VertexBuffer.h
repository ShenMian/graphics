// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Buffer.h"
#include "VertexFormat.h"
#include <math/math.hpp>
#include <memory>
#include <vector>

/**
 * @brief 顶点缓冲区.
 */
class VertexBuffer
{
public:
	/**
	 * @brief 创建 VertexBuffer.
	 *
	 * @param data  顶点缓冲区.
	 * @param fmt   顶点格式.
	 * @param usage 使用方式.
	 */
	template <typename T>
	[[nodiscard]] static std::shared_ptr<VertexBuffer> create(const std::vector<T>& data, const VertexFormat& fmt,
	                                                          Buffer::Usage usage = Buffer::Usage::Static);

	/**
	 * @brief 创建 VertexBuffer.
	 *
	 * @param data  顶点缓冲区指针.
	 * @param size  顶点缓冲区大小, 单位: 字节.
	 * @param count 顶点数量.
	 * @param fmt   顶点格式.
	 * @param usage 使用方式.
	 */
	[[nodiscard]] static std::shared_ptr<VertexBuffer> create(const void* data, size_t size, const VertexFormat& fmt,
	                                                          Buffer::Usage usage = Buffer::Usage::Static);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	[[nodiscard]] size_t getSize() const;

	/**
	 * @brief 获取顶点数.
	 */
	[[nodiscard]] uint32_t getCount() const;

	/**
	 * @brief 获取顶格式.
	 */
	[[nodiscard]] const VertexFormat& getFormat() const;

	virtual Buffer& getBuffer() = 0;

protected:
	VertexBuffer(const void* data, size_t size, const VertexFormat& layout);

private:
	size_t       size;
	uint32_t     count;
	VertexFormat format;
};

template <typename T>
inline std::shared_ptr<VertexBuffer> VertexBuffer::create(const std::vector<T>& data, const VertexFormat& fmt,
                                                          Buffer::Usage usage)
{
	return create(data.data(), data.size() * sizeof(T), fmt, usage);
}
