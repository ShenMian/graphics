// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Texture.h"
#include "Renderer.h"

#include "OpenGL/GLTexture.h"

std::shared_ptr<Texture> Texture::create(const Image& image, Type type)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLTexture>(image, type);
	}
	return nullptr;
}

Texture::Type Texture::getType() const
{
	return type;
}

Format Texture::getFormat() const
{
	return format;
}

Texture::Texture(Type type, Format fmt)
	: type(type), format(fmt)
{
}
