// Copyright 2021 SMS
// License(Apache-2.0)

#include "CommandBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLCommandBuffer.h"

std::shared_ptr<CommandBuffer> CommandBuffer::create()
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLCommandBuffer>();
	}
	return nullptr;
}