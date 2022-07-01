// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "UniformBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLUniformBuffer.h"
#include "Vulkan/VKUniformBuffer.h"

std::shared_ptr<UniformBuffer> UniformBuffer::create(int binding, size_t size)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLUniformBuffer>(binding, size);

	case Vulkan:
		return std::make_shared<VKUniformBuffer>(binding, size);
	}
	return nullptr;
}

UniformBuffer::UniformBuffer(int binding)
	: binding(binding)
{
}
