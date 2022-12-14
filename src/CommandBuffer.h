// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include <functional>
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

	/**
	 * @brief 开始记录命令.
	 */
	virtual void begin() = 0;

	/**
	 * @brief 结束记录命令.
	 */
	virtual void end() = 0;

	virtual void beginRenderPass() = 0;
	virtual void endRenderPass()   = 0;

	virtual void setViewport(const Viewport& viewport)                           = 0;
	virtual void setPipeline(std::shared_ptr<Pipeline> pipeline)                 = 0;
	virtual void setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)     = 0;
	virtual void setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)        = 0;
	virtual void setTexture(std::shared_ptr<Texture> texture, unsigned int slot) = 0;

	/**
	 * @brief 清空缓冲区.
	 *
	 * @param flags 类型.
	 *
	 * @see ClearFlag
	 */
	virtual void clear(uint8_t flags) = 0;

	/**
	 * @brief 设置清空颜色缓冲区的默认值.
	 *
	 * @param color 默认值.
	 */
	virtual void setClearColor(const Vector4& color) = 0;

	/**
	 * @brief 设置清空深度缓冲区的默认值.
	 *
	 * @param depth 默认值.
	 */
	virtual void setClearDepth(float depth) = 0;

	/**
	 * @brief 设置清空模板缓冲区的默认值.
	 *
	 * @param value 默认值.
	 */
	virtual void setClearStencil(uint32_t value) = 0;

	virtual void draw(uint32_t vertexCount, uint32_t firstVertex = 0)      = 0;
	virtual void drawIndexed(uint32_t indexCount, uint32_t firstIndex = 0) = 0;
};
