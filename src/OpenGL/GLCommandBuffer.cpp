// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "GLCommandBuffer.h"
#include "../Viewport.hpp"
#include <glad/glad.h>

namespace
{

std::unordered_map<uint8_t, GLbitfield> GLClearFlag = {{ClearFlag::Color, GL_COLOR_BUFFER_BIT},
                                                       {ClearFlag::Depth, GL_DEPTH_BUFFER_BIT},
                                                       {ClearFlag::Stencil, GL_STENCIL_BUFFER_BIT}};

}


void GLCommandBuffer::begin()
{
	buffer.clear();
}

void GLCommandBuffer::end()
{
}

void GLCommandBuffer::begin_render_pass()
{
}

void GLCommandBuffer::end_render_pass()
{
}

void GLCommandBuffer::set_viewport(const Viewport& viewport)
{
	auto cmd    = add_command<GLCmdSetViewport>(GLOpcode::setViewport);
	cmd->x      = static_cast<GLint>(viewport.x);
	cmd->y      = static_cast<GLint>(viewport.x);
	cmd->width  = static_cast<GLsizei>(viewport.width);
	cmd->height = static_cast<GLsizei>(viewport.height);
	cmd->n      = static_cast<GLfloat>(viewport.minDepth);
	cmd->f      = static_cast<GLfloat>(viewport.maxDepth);
}

void GLCommandBuffer::set_pipeline(std::shared_ptr<Pipeline> pipeline)
{
	auto cmd      = add_command<GLCmdSetPipeline>(GLOpcode::setPipeline);
	cmd->pipeline = pipeline;
}

void GLCommandBuffer::set_vertex_buffer(std::shared_ptr<VertexBuffer> vertex_buffer)
{
	auto cmd          = add_command<GLCmdSetVertexBuffer>(GLOpcode::setVertexBuffer);
	cmd->vertexBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(vertex_buffer);
}

void GLCommandBuffer::set_index_buffer(std::shared_ptr<IndexBuffer> index_buffer)
{
	auto cmd         = add_command<GLCmdSetIndexBuffer>(GLOpcode::setIndexBuffer);
	cmd->indexBuffer = std::dynamic_pointer_cast<GLIndexBuffer>(index_buffer);
}

void GLCommandBuffer::set_texture(std::shared_ptr<Texture> texture, unsigned int slot)
{
	auto cmd     = add_command<GLCmdSetTexture>(GLOpcode::setTexture);
	cmd->texture = std::dynamic_pointer_cast<GLTexture>(texture);
	cmd->slot    = slot;
}

void GLCommandBuffer::clear(uint8_t flags)
{
	auto cmd   = add_command<GLCmdClear>(GLOpcode::clear);
	cmd->flags = flags;
}

void GLCommandBuffer::set_clear_color(const Vector4& color)
{
	auto cmd   = add_command<GLCmdSetClearColor>(GLOpcode::setClearColor);
	cmd->color = color;
}

void GLCommandBuffer::set_clear_depth(float depth)
{
	auto cmd   = add_command<GLCmdSetClearDepth>(GLOpcode::setClearDepth);
	cmd->depth = depth;
}

void GLCommandBuffer::set_clear_stencil(uint32_t value)
{
	auto cmd = add_command<GLCmdSetClearStencil>(GLOpcode::setClearStencil);
	cmd->s   = value;
}

void GLCommandBuffer::draw(uint32_t vertex_count, uint32_t first_vertex)
{
	auto cmd         = add_command<GLCmdDraw>(GLOpcode::draw);
	cmd->vertexCount = vertex_count;
	cmd->firstVertex = first_vertex;
}

void GLCommandBuffer::draw_indexed(uint32_t index_count, uint32_t first_index)
{
	auto cmd        = add_command<GLCmdDrawIndexed>(GLOpcode::drawIndexed);
	cmd->indexCount = index_count;
	cmd->firstIndex = first_index;
}

void GLCommandBuffer::add_command(GLOpcode opcode)
{
	buffer.push_back(static_cast<uint8_t>(opcode));
}
