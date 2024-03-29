// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "CommandQueue.h"
#include "Renderer.h"
#include <cassert>

#include "OpenGL/GLCommandQueue.h"
#include "Vulkan/VKCommandQueue.h"

std::shared_ptr<CommandQueue> CommandQueue::create()
{
	switch(Renderer::get_backend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		return std::make_shared<GLCommandQueue>();

	case Vulkan:
		return std::make_shared<VKCommandQueue>();

	default:
		assert(false);
	}
	return nullptr;
}
