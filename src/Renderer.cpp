// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Renderer.h"
#include <cassert>

#include "OpenGL/GLRenderer.h"
#include "Vulkan/VKRenderer.h"

Renderer::Backend Renderer::backend = Renderer::Backend::OpenGL;

Renderer* Renderer::get()
{
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL: {
		static auto renderer = new GLRenderer; // TODO: 内存泄露
		return renderer;
	}

	case Vulkan: {
		static auto renderer = new VKRenderer; // TODO: 内存泄露
		return renderer;
	}
	}
	return nullptr;
}

void Renderer::setBackend(Backend newBackend)
{
	backend = newBackend;
}

Renderer::Backend Renderer::getBackend()
{
	return backend;
}

void Renderer::init(const Window& win)
{
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		GLRenderer::init(win);
		break;

	case Vulkan:
		VKRenderer::init(win);
		break;
	}
}

void Renderer::deinit()
{
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		GLRenderer::deinit();
		break;

	case Vulkan:
		VKRenderer::deinit();
		break;
	}
}
