// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Texture.h"
#include "Renderer.h"
#include "Core/Image.h"
#include <stdexcept>

#include "OpenGL/GLTexture.h"

std::shared_ptr<Texture> Texture::create(const Image& image, Type type)
{
	if(type == Type::Cube)
		throw std::runtime_error("cubemap should have 6 images");

	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLTexture>(image, type);
	}
	return nullptr;
}

std::shared_ptr<Texture> Texture::create(const std::vector<Image>& images)
{
	if(images.size() != 6)
		throw std::runtime_error("cubemap should have 6 images");

	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLTexture>(images);
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
