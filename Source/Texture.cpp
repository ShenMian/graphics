// Copyright 2021 SMS
// License(Apache-2.0)

#include "Texture.h"
#include "Renderer.h"

#include "OpenGL/GLTexture.h"

std::shared_ptr<Texture> Texture::create(const Image& image)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLTexture>(image);
	}
	return nullptr;
}
