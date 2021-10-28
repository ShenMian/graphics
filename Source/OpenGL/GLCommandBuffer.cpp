// Copyright 2021 SMS
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

enum class GLOpcode
{
	setViewport,

	clear,
	setClearColor,
	setClearDepth,
	setClearStencil
};


struct GLCmdSetViewport
{
	Vector2i origin;
	Vector2i size;
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


void GLCommandBuffer::begin()
{
	buffer.clear();
}

void GLCommandBuffer::end()
{
}

// TODO: 移动到 GLCommandQueue::submit()
void GLCommandBuffer::execute()
{
	const auto end = buffer.data() + buffer.size();
	auto        pc = buffer.data();

	while(pc < end)
	{
		const auto opcode = *reinterpret_cast<const GLOpcode*>(pc);
		pc += sizeof(GLOpcode);
		pc += execute(opcode, pc);
	}
}

void GLCommandBuffer::setViewport(const Vector2i& origin, const Vector2i& size)
{
	auto cmd    = addCommand<GLCmdSetViewport>(GLOpcode::setViewport);
	cmd->origin = origin;
	cmd->size   = size;
}

void GLCommandBuffer::clear(uint8_t flags)
{
	auto cmd   = addCommand<GLCmdClear>(GLOpcode::clear);
	cmd->flags = flags;
}

void GLCommandBuffer::setClearColor(const Vector4& color)
{
	auto cmd   = addCommand<GLCmdSetClearColor>(GLOpcode::setClearColor);
	cmd->color = color;
}

void GLCommandBuffer::setClearDepth(float depth)
{
	auto cmd   = addCommand<GLCmdSetClearDepth>(GLOpcode::setClearDepth);
	cmd->depth = depth;
}

void GLCommandBuffer::setClearStencil()
{
}

size_t GLCommandBuffer::execute(GLOpcode opcode, uint8_t* pc)
{
	switch(opcode)
	{
	case GLOpcode::setViewport:
	{
		auto args = reinterpret_cast<GLCmdSetViewport*>(pc);
		glViewport(args->origin.x, args->origin.y, args->size.x, args->size.y);
		return sizeof(*args);
	}

	case GLOpcode::clear:
	{
		auto args = reinterpret_cast<GLCmdClear*>(pc);
		GLbitfield glFlags = 0;
		if(args->flags & ClearFlag::Color)
			glFlags |= GL_COLOR_BUFFER_BIT;
		if(args->flags & ClearFlag::Depth)
			glFlags |= GL_DEPTH_BUFFER_BIT;
		if(args->flags & ClearFlag::Stencil)
			glFlags |= GL_STENCIL_BUFFER_BIT;
		glClear(glFlags);
		return sizeof(*args);
	}

	case GLOpcode::setClearColor:
	{
		const auto args = reinterpret_cast<GLCmdSetClearColor*>(pc);
		glClearColor(args->color.x, args->color.y, args->color.z, args->color.w);
		return sizeof(*args);
	}

	case GLOpcode::setClearDepth:
	{
		const auto args = reinterpret_cast<GLCmdSetClearDepth*>(pc);
		glClearDepth(args->depth);
		return sizeof(*args);
	}

	default:
		assert(false);
	}
}

void GLCommandBuffer::addCommand(GLOpcode opcode)
{
	buffer.push_back(static_cast<uint8_t>(opcode));
}