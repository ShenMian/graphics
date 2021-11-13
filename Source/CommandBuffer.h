// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include <Math/Vector4.hpp>
#include <memory>

class VertexBuffer;
class IndexBuffer;

struct ClearFlag
{
	enum
	{
		Color   = (1 << 0),
		Depth   = (1 << 1),
		Stencil = (1 << 2)
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

	virtual void end() = 0;

	// virtual void execute(std::shared_ptr<CommandBuffer> commandBuffer) = 0;

	virtual void setViewport(const Vector2i& origin, const Vector2i& size) = 0;

	virtual void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
	virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer) = 0;

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
	virtual void clear(uint8_t flags) = 0;
	virtual void setClearColor(const Vector4& color) = 0;
	virtual void setClearDepth(float depth) = 0;
	virtual void setClearStencil() = 0;

	virtual void draw(size_t first, size_t num) = 0;

private:
	std::shared_ptr<Program> program;
};