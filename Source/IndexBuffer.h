// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <memory>
#include <vector>

/**
 * @brief 索引缓冲区.
 */
class IndexBuffer
{
public:
	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  索引缓冲区.
	 * @param usage 使用方式.
	 */
	template <typename T>
	static std::shared_ptr<IndexBuffer> create(const std::vector<T>& data, Usage usage = Usage::Static);

	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  索引缓冲区指针.
	 * @param size  索引缓冲区大小, 单位: 字节.
	 * @param count 索引数量.
	 * @param usage 使用方式.
	 */
	static std::shared_ptr<IndexBuffer> create(const void* data, size_t size, size_t count, Usage usage = Usage::Static);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	size_t getSize() const;

	/**
	 * @brief 获取索引数.
	 */
	size_t getCount() const;

	virtual void bind() = 0;

protected:
	IndexBuffer(size_t size, size_t count);

private:
	size_t size;
	size_t count;
};

template <typename T>
inline std::shared_ptr<IndexBuffer> IndexBuffer::create(const std::vector<T>& data, Usage usage)
{
	return create(data.data(), data.size() * sizeof(T), data.size(), usage);
}