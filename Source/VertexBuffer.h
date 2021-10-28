// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <Math/Vector2.hpp>
#include <Math/Vector3.hpp>
#include <memory>
#include <vector>

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;
	Vector3 tangent;
	Vector3 bitangent;
};

class VertexBuffer
{
public:
	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

	/**
	 * @brief 创建 VertexBuffer.
	 *
	 * @param data  顶点缓冲区.
	 * @param usage 使用方式.
	 */
	template <typename T>
	static std::shared_ptr<VertexBuffer> create(const std::vector<T>& data, Usage usage = Usage::Static);

	/**
	 * @brief 创建 VertexBuffer.
	 *
	 * @param data  顶点缓冲区指针.
	 * @param size  顶点缓冲区大小, 单位: 字节.
	 * @param count 顶点数量.
	 * @param usage 使用方式.
	 */
	static std::shared_ptr<VertexBuffer> create(const void* data, size_t size, size_t count, Usage usage = Usage::Static);

	/**
	 * @brief 获取缓冲区大小, 单位: 字节.
	 */
	size_t getSize() const;

	/**
	 * @brief 获取顶点数.
	 */
	size_t getCount() const;

	virtual void bind() = 0;

protected:
	VertexBuffer(size_t size, size_t count);

private:
	size_t size;
	size_t count;
};

template <typename T>
inline std::shared_ptr<VertexBuffer> VertexBuffer::create(const std::vector<T>& data, Usage usage)
{
	return create(data.data(), data.size() * sizeof(T), data.size(), usage);
}