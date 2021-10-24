// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <memory>
#include <vector>

class IndexBuffer
{
public:
	using value_type = unsigned int;

	enum class Usage
	{
		Static,
		Dynamic,
		Stream
	};

	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  顶点缓冲区.
	 * @param usage 使用方式.
	 * 
	 * @note 可能会被弃用.
	 */
	static std::shared_ptr<IndexBuffer> create(const std::vector<value_type>& data, Usage usage = Usage::Static);

	/**
	 * @brief 创建 IndexBuffer.
	 *
	 * @param data  顶点缓冲区指针.
	 * @param size  顶点缓冲区大小.
	 * @param usage 使用方式.
	 */
	static std::shared_ptr<IndexBuffer> create(const void* data, size_t size, Usage usage = Usage::Static);

	virtual void bind() = 0;
};