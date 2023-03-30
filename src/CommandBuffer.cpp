// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "CommandBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLCommandBuffer.h"
#include "Vulkan/VKCommandBuffer.h"

std::shared_ptr<CommandBuffer> CommandBuffer::create()
{
	switch(Renderer::get_backend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		return std::make_shared<GLCommandBuffer>();

	case Vulkan:
		return std::make_shared<VKCommandBuffer>();
	}
	return nullptr;
}
