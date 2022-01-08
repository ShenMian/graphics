// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "CommandBuffer.h"
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

	void setViewport(const Vector2f& origin, const Vector2f& size, const Vector2f& depth) override;

	void setPipeline(std::shared_ptr<Pipeline> pipeline) override;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;

	void clear(uint8_t flags) override;
	void setClearColor(const Vector4& color) override;
	void setClearDepth(float depth) override;
	void setClearStencil() override;

	void draw(uint32_t vertexCount, uint32_t firstVertex) override;
	void drawIndexed(uint32_t indexCount, uint32_t firstIndex) override;

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

	setPipeline,

	setVertexBuffer,
	setIndexBuffer,

	clear,
	setClearColor,
	setClearDepth,
	setClearStencil,

	draw,
	drawIndexed
};

struct GLCmdSetViewport
{
	GLint x, y;
	GLsizei width, height;
	GLdouble n, f;
};

struct GLCmdSetPipeline
{
	std::shared_ptr<Pipeline> pipeline;
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
	uint32_t firstVertex;
	uint32_t vertexCount;
};

struct GLCmdDrawIndexed
{
	uint32_t firstIndex;
	uint32_t indexCount;
};
