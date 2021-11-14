// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLCommandQueue.h"
#include "GLCommandBuffer.h"
#include <glad/glad.h>

void GLCommandQueue::submit(std::shared_ptr<CommandBuffer> commandBuffer)
{
	auto& buffer = dynamic_cast<GLCommandBuffer*>(commandBuffer.get())->getData();

	const auto end = buffer.data() + buffer.size();
	auto        pc = buffer.data();

	while(pc < end)
	{
		const auto opcode = *reinterpret_cast<const GLOpcode*>(pc);
		pc += sizeof(GLOpcode);
		pc += execute(opcode, pc);
	}
}

size_t GLCommandQueue::execute(GLOpcode opcode, const uint8_t* pc)
{
	switch(opcode)
	{
	case GLOpcode::setViewport:
	{
		auto args = reinterpret_cast<const GLCmdSetViewport*>(pc);
		glViewport(args->origin.x, args->origin.y, args->size.x, args->size.y);
		return sizeof(*args);
	}

	case GLOpcode::setVertexBuffer:
	{
		auto args = reinterpret_cast<const GLCmdSetVertexBuffer*>(pc);
		args->vertexBuffer->bind();
		return sizeof(*args);
	}

	case GLOpcode::setIndexBuffer:
	{
		auto args = reinterpret_cast<const GLCmdSetIndexBuffer*>(pc);
		args->indexBuffer->bind();
		return sizeof(*args);
	}

	case GLOpcode::clear:
	{
		auto args = reinterpret_cast<const GLCmdClear*>(pc);
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
		const auto args = reinterpret_cast<const GLCmdSetClearColor*>(pc);
		glClearColor(args->color.x, args->color.y, args->color.z, args->color.w);
		return sizeof(*args);
	}

	case GLOpcode::setClearDepth:
	{
		const auto args = reinterpret_cast<const GLCmdSetClearDepth*>(pc);
		glClearDepth(args->depth);
		return sizeof(*args);
	}
	
	case GLOpcode::draw:
	{
		const auto args = reinterpret_cast<const GLCmdDraw*>(pc);
		glDrawArrays(GL_TRIANGLES, (GLsizei)args->first, (GLsizei)args->num);
		return sizeof(*args);
	}

	default:
		assert(false);
	}
	return 0;
}
