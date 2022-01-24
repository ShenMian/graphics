// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLCommandBuffer.h"
#include "../Viewport.hpp"
#include <glad/glad.h>

namespace
{

std::unordered_map<uint8_t, GLbitfield> GLClearFlag = {
	{ClearFlag::Color, GL_COLOR_BUFFER_BIT},
	{ClearFlag::Depth, GL_DEPTH_BUFFER_BIT},
	{ClearFlag::Stencil, GL_STENCIL_BUFFER_BIT}
};

}


void GLCommandBuffer::begin()
{
	buffer.clear();
}

void GLCommandBuffer::end()
{
}

void GLCommandBuffer::beginRenderPass(std::shared_ptr<Pipeline> pipeline)
{
}

void GLCommandBuffer::endRenderPass()
{
}

void GLCommandBuffer::setViewport(const Viewport& viewport)
{
	auto cmd = addCommand<GLCmdSetViewport>(GLOpcode::setViewport);
	cmd->x = static_cast<GLint>(viewport.x);
	cmd->y = static_cast<GLint>(viewport.x);
	cmd->width = static_cast<GLsizei>(viewport.width);
	cmd->height = static_cast<GLsizei>(viewport.height);
	cmd->n = static_cast<GLdouble>(viewport.minDepth);
	cmd->f = static_cast<GLdouble>(viewport.maxDepth);
}

void GLCommandBuffer::setPipeline(std::shared_ptr<Pipeline> pipeline)
{
	auto cmd = addCommand<GLCmdSetPipeline>(GLOpcode::setPipeline);
	cmd->pipeline = pipeline;
}

void GLCommandBuffer::setVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer)
{
	auto cmd = addCommand<GLCmdSetVertexBuffer>(GLOpcode::setVertexBuffer);
	cmd->vertexBuffer = vertexBuffer;
}

void GLCommandBuffer::setIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer)
{
	auto cmd = addCommand<GLCmdSetIndexBuffer>(GLOpcode::setIndexBuffer);
	cmd->indexBuffer = indexBuffer;
}

void GLCommandBuffer::clear(uint8_t flags)
{
	auto cmd = addCommand<GLCmdClear>(GLOpcode::clear);
	cmd->flags = flags;
}

void GLCommandBuffer::setClearColor(const Vector4& color)
{
	auto cmd = addCommand<GLCmdSetClearColor>(GLOpcode::setClearColor);
	cmd->color = color;
}

void GLCommandBuffer::setClearDepth(float depth)
{
	auto cmd = addCommand<GLCmdSetClearDepth>(GLOpcode::setClearDepth);
	cmd->depth = depth;
}

void GLCommandBuffer::setClearStencil()
{
}

void GLCommandBuffer::draw(uint32_t vertexCount, uint32_t firstVertex)
{
	auto cmd = addCommand<GLCmdDraw>(GLOpcode::draw);
	cmd->vertexCount = vertexCount;
	cmd->firstVertex = firstVertex;
}

void GLCommandBuffer::drawIndexed(uint32_t indexCount, uint32_t firstIndex)
{
	auto cmd = addCommand<GLCmdDrawIndexed>(GLOpcode::drawIndexed);
	cmd->indexCount = indexCount;
	cmd->firstIndex = firstIndex;
}

void GLCommandBuffer::addCommand(GLOpcode opcode)
{
	buffer.push_back(static_cast<uint8_t>(opcode));
}
