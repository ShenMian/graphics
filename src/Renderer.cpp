// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Renderer.h"
#include <cassert>

#include "OpenGL/GLRenderer.h"
#include "Vulkan/VKRenderer.h"

Renderer::Backend Renderer::backend_ = Renderer::Backend::OpenGL;

Renderer* Renderer::get()
{
	switch(Renderer::get_backend())
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

void Renderer::set_backend(Backend newBackend)
{
	backend_ = newBackend;
}

Renderer::Backend Renderer::get_backend()
{
	return backend_;
}

void Renderer::init(const Window& win)
{
	switch(Renderer::get_backend())
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
	switch(Renderer::get_backend())
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
