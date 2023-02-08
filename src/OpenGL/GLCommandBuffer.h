// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "CommandBuffer.h"
#include "GLIndexBuffer.h"
#include "GLTexture.h"
#include "GLVertexBuffer.h"
#include <cstdint>
#include <vector>

enum class GLOpcode;

class GLCommandBuffer : public CommandBuffer
{
public:
	void begin() override;
	void end() override;

	void beginRenderPass() override;
	void endRenderPass() override;

	void setViewport(const Viewport& viewport) override;

	void setPipeline(std::shared_ptr<Pipeline> pipeline) override;

	void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) override;
	void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) override;
	void setTexture(std::shared_ptr<Texture> texture, unsigned int slot) override;

	void clear(uint8_t flags) override;
	void setClearColor(const Vector4& color) override;
	void setClearDepth(float depth) override;
	void setClearStencil(uint32_t value) override;

	void draw(uint32_t vertexCount, uint32_t firstVertex) override;
	void drawIndexed(uint32_t indexCount, uint32_t firstIndex) override;

	const auto& getData() const;

private:
	void addCommand(GLOpcode opcode);
	template <typename T>
	T* addCommand(GLOpcode opcode);

	std::vector<uint8_t> buffer;
};

inline const auto& GLCommandBuffer::getData() const
{
	return buffer;
}

template <typename T>
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
	setTexture,

	clear,
	setClearColor,
	setClearDepth,
	setClearStencil,

	draw,
	drawIndexed
};

struct GLCmdSetViewport
{
	GLint   x, y;
	GLsizei width, height;
	GLfloat n, f;
};

struct GLCmdSetPipeline
{
	std::shared_ptr<Pipeline> pipeline;
};

struct GLCmdSetVertexBuffer
{
	std::shared_ptr<GLVertexBuffer> vertexBuffer;
};

struct GLCmdSetIndexBuffer
{
	std::shared_ptr<GLIndexBuffer> indexBuffer;
};

struct GLCmdSetTexture
{
	std::shared_ptr<GLTexture> texture;
	unsigned int               slot;
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

struct GLCmdSetClearStencil
{
	int s;
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
