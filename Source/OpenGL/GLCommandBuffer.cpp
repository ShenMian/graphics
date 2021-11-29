// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLCommandBuffer.h"
#include <glad/glad.h>

namespace
{

std::unordered_map<uint8_t, GLbitfield> GLClearFlag = {
	{ClearFlag::Color, GL_COLOR_BUFFER_BIT},
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

void GLCommandBuffer::setViewport(const Vector2i& origin, const Vector2i& size)
{
	auto cmd = addCommand<GLCmdSetViewport>(GLOpcode::setViewport);
	cmd->origin = origin;
	cmd->size = size;
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

void GLCommandBuffer::draw(size_t first, size_t num)
{
	auto cmd = addCommand<GLCmdDraw>(GLOpcode::draw);
	cmd->first = first;
	cmd->num = num;
}

void GLCommandBuffer::addCommand(GLOpcode opcode)
{
	buffer.push_back(static_cast<uint8_t>(opcode));
}
