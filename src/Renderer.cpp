﻿// Copyright 2021 ShenMian
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

void Renderer::setAPI(API newAPI)
{
	api = newAPI;
}

Renderer::API Renderer::getAPI()
{
	return api;
}

void Renderer::init(const Window& win)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

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
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		GLRenderer::deinit();
		break;

	case Vulkan:
		VKRenderer::deinit();
		break;
	}
}
