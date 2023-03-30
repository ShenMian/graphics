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

	virtual void begin_render_pass() = 0;
	virtual void end_render_pass()   = 0;

	virtual void set_viewport(const Viewport& viewport)                           = 0;
	virtual void set_pipeline(std::shared_ptr<Pipeline> pipeline)                 = 0;
	virtual void set_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer)     = 0;
	virtual void set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer)        = 0;
	virtual void set_texture(std::shared_ptr<Texture> texture, unsigned int slot) = 0;

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
	virtual void set_clear_color(const Vector4& color) = 0;

	/**
	 * @brief 设置清空深度缓冲区的默认值.
	 *
	 * @param depth 默认值.
	 */
	virtual void set_clear_depth(float depth) = 0;

	/**
	 * @brief 设置清空模板缓冲区的默认值.
	 *
	 * @param value 默认值.
	 */
	virtual void set_clear_stencil(uint32_t value) = 0;

	virtual void draw(uint32_t vertex_count, uint32_t first_vertex = 0)      = 0;
	virtual void draw_indexed(uint32_t index_count, uint32_t first_index = 0) = 0;
};
