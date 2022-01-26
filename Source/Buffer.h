// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <cstddef>

/**
 * @brief 缓冲区.
 */
class Buffer
{
public:
	/**
	 * @brief 缓冲区类型. 表明了缓冲区的用途.
	 */
	enum class Type
	{
		Vertex,
		Index,
		Uniform
	};

	/**
	 * @brief 缓冲区使用方式.
	 */
	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

	/**
	 * @brief 构造函数.
	 *
	 * @param size  缓冲区大小, 单位: 字节.
	 * @param type  缓冲区类型.
	 * @param usage 缓冲区使用方式.
	 */
	Buffer(size_t size, Type type, Usage usage);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	size_t getSize() const;

	/**
	 * @brief 获取缓冲区类型.
	 */
	Type getType() const;

	/**
	 * @brief 获取缓冲区使用方式.
	 */
	Usage getUsage() const;

	/**
	 * @brief 获取映射的缓冲区.
	 */
	void* getData();

	void write(const void* data, size_t size, size_t offset = 0);

	void read(void* data, size_t size, size_t offset = 0);

	/**
	 * @brief 映射缓冲区到内存.
	 *
	 * @param size   要映射的大小, 默认值: 全部.
	 * @param offset 偏移量, 单位: byte.
	 */
	virtual void map(size_t size = -1, size_t offset = 0) = 0;

	/**
	 * @brief 取消映射.
	 */
	virtual void unmap() = 0;

	/**
	 * @brief 刷新映射缓冲区.
	 *
	 * @param size   要刷新的大小, 默认值: 全部.
	 * @param offset 偏移量, 单位: byte.
	 */
	virtual void flush(size_t size = -1, size_t offset = 0) = 0;

protected:
	size_t size;
	Type   type;
	Usage  usage;
	void* data = nullptr;
};
