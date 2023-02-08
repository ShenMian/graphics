// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <cstddef>
#include <span>

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
	 * @brief 写入映射缓冲区.
	 *
	 * @param data   要写入的数据.
	 * @param offset 偏移量, 单位: byte.
	 */
	template <typename T>
	void write(std::span<T> data, size_t offset = 0);

	/**
	 * @brief 写入映射缓冲区.
	 *
	 * @param data   要写入的数据.
	 * @param size   要写入的大小, 单位: byte.
	 * @param offset 偏移量, 单位: byte.
	 */
	void write(const void* data, size_t size, size_t offset = 0);

	/**
	 * @brief 读取映射缓冲区.
	 *
	 * @param data   要写入的缓冲区.
	 * @param size   要读取的大小, 单位: byte.
	 * @param offset 偏移量, 单位: byte.
	 */
	void read(void* data, size_t size, size_t offset = 0);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	[[nodiscard]] size_t getSize() const;

	/**
	 * @brief 获取缓冲区类型.
	 */
	[[nodiscard]] Type getType() const;

	/**
	 * @brief 获取缓冲区使用方式.
	 */
	[[nodiscard]] Usage getUsage() const;

	/**
	 * @brief 获取映射的缓冲区.
	 */
	[[nodiscard]] void* getData();

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
	void*  data = nullptr;
};

template <typename T>
void Buffer::write(std::span<T> data, size_t offset)
{
	write(data.data(), data.size_bytes(), offset);
}
