// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Renderer.h"
#include <cassert>

#include "OpenGL/GLRenderer.h"
#include "Vulkan/VKRenderer.h"

Renderer::API Renderer::api = Renderer::API::OpenGL;

Renderer* Renderer::get()
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
	{
		static auto renderer = new GLRenderer;
		return renderer;
	}

	case Vulkan:
	{
		VKRenderer::init();
		static auto renderer = new VKRenderer;
		return renderer;
	}
	}
	return nullptr;
}

void Renderer::setAPI(API newAPI)
{
	api = newAPI;
}

Renderer::API Renderer::getAPI()
{
	return api;
}
