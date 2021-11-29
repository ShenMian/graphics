// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../CommandBuffer.h"
#include "GLVertexBuffer.h"
#include "GLIndexBuffer.h"
#include <cstdint>
#include <vector>

enum class GLOpcode;

class GLCommandBuffer : public CommandBuffer
{
public:
	void begin() override;
	void end() override;

	void setViewport(const Vector2i& origin, const Vector2i& size) override;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

	void clear(uint8_t flags) override;
	void setClearColor(const Vector4& color) override;
	void setClearDepth(float depth) override;
	void setClearStencil() override;

	void draw(size_t first, size_t num) override;

	const auto& getData() const;

private:
	void addCommand(GLOpcode opcode);
	template <typename T> T* addCommand(GLOpcode opcode);

	std::vector<uint8_t> buffer;
};

inline const auto& GLCommandBuffer::getData() const
{
	return buffer;
}

template<typename T>
inline T* GLCommandBuffer::addCommand(GLOpcode opcode)
{
	auto offset = buffer.size();
	buffer.push_back(static_cast<uint8_t>(opcode));
	buffer.resize(offset + sizeof(GLOpcode) + sizeof(T));
	return reinterpret_cast<T*>(&buffer[offset + sizeof(GLOpcode)]);
}

enum class GLOpcode
{
	setViewport,

	setVertexBuffer,
	setIndexBuffer,

	clear,
	setClearColor,
	setClearDepth,
	setClearStencil,

	draw
};

struct GLCmdSetViewport
{
	Vector2i origin;
	Vector2i size;
};

struct GLCmdSetVertexBuffer
{
	std::shared_ptr<VertexBuffer> vertexBuffer;
};

struct GLCmdSetIndexBuffer
{
	std::shared_ptr<IndexBuffer> indexBuffer;
};

struct GLCmdClear
{
	uint8_t flags;
};

struct GLCmdSetClearColor
{
	Vector4 color;
};

struct GLCmdSetClearDepth
{
	float depth;
};

struct GLCmdDraw
{
	size_t first;
	size_t num;
};
