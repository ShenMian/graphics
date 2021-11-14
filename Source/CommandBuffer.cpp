// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "CommandBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLCommandBuffer.h"
#include "Vulkan/VKCommandBuffer.h"

std::shared_ptr<CommandBuffer> CommandBuffer::create()
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLCommandBuffer>();

	case Vulkan:
		return std::make_shared<VKCommandBuffer>();
	}
	return nullptr;
}