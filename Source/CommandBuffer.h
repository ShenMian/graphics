﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include <math/math.hpp>
#include <memory>

class Texture;
class Pipeline;
class VertexBuffer;
class IndexBuffer;
struct Viewport;

struct ClearFlag
{
	enum
	{
		Color   = 1 << 0,
		Depth   = 1 << 1,
		Stencil = 1 << 2
	};
};

/**
 * @brief 命令缓冲区.
 */
class CommandBuffer
{
public:
	static std::shared_ptr<CommandBuffer> create();

	virtual void begin() = 0;
	virtual void end()   = 0;

	virtual void beginRenderPass() = 0;
	virtual void endRenderPass()   = 0;

	virtual void setViewport(const Viewport& viewport) = 0;

	virtual void setPipeline(std::shared_ptr<Pipeline> pipeline) = 0;

	virtual void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)     = 0;
	virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)        = 0;
	virtual void setTexture(std::shared_ptr<Texture> texture, unsigned int slot) = 0;

	/**
	 * @brief 清空缓冲区.
	 *
	 * @param flags 类型.
	 *
	 * @see ClearFlag
	 * @see setClearColor
	 * @see setClearDepth
	 * @see setClearStencil
	 */
	virtual void clear(uint8_t flags)                = 0;
	virtual void setClearColor(const Vector4& color) = 0;
	virtual void setClearDepth(float depth)          = 0;
	virtual void setClearStencil()                   = 0;

	virtual void draw(uint32_t vertexCount, uint32_t firstVertex = 0)      = 0;
	virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex = 0) = 0;
};
