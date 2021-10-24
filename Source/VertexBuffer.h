// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <memory>
#include <vector>

struct Vector2
{
	float x = 0.f;
	float y = 0.f;
};

struct Vector3
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};

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
	using value_type = Vertex;

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
	static std::shared_ptr<VertexBuffer> create(const std::vector<value_type>& data, Usage usage = Usage::Static);

	/**
	 * @brief 创建 VertexBuffer.
	 *
	 * @param data  顶点缓冲区指针.
	 * @param size  顶点缓冲区大小.
	 * @param usage 使用方式.
	 */
	static std::shared_ptr<VertexBuffer> create(const void* data, size_t size, Usage usage = Usage::Static);
};