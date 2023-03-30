// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "../CommandBuffer.h"
#include <vector>
#include <vulkan/vulkan.h>

class VKCommandBuffer : public CommandBuffer
{
public:
	VKCommandBuffer();
	virtual ~VKCommandBuffer();

	void begin() override;
	void end() override;

	void begin_render_pass() override;
	void end_render_pass() override;

	void set_viewport(const Viewport& viewport) override;

	void set_pipeline(std::shared_ptr<Pipeline> pipeline) override;

	void set_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer) override;
	void set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer) override;
	void set_texture(std::shared_ptr<Texture> texture, unsigned int slot) override;

	void clear(uint8_t flags) override;
	void set_clear_color(const Vector4& color) override;
	void set_clear_depth(float depth) override;
	void set_clear_stencil(uint32_t value) override;

	void draw(uint32_t vertex_count, uint32_t first_vertex) override;
	void draw_indexed(uint32_t index_count, uint32_t first_index) override;

	operator VkCommandBuffer() const;

private:
	std::vector<VkCommandBuffer> handles_;
	size_t                       index_ = 0;

	VkClearColorValue clear_color_ = {};
};
