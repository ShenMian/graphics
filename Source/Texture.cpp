// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Texture.h"
#include "Renderer.h"
#include "Core/Image.h"
#include <stdexcept>

#include "OpenGL/GLTexture.h"

namespace fs = std::filesystem;

std::unordered_map<fs::path, std::shared_ptr<Texture>> Texture::cache;

std::shared_ptr<Texture> Texture::create(const fs::path& path, Type type)
{
	if(type == Type::Cube)
		throw std::runtime_error("cubemap should have 6 file path");

	const auto absPath = fs::absolute(path);
	const auto it = cache.find(absPath);
	if(it != cache.end())
		return it->second;

	std::shared_ptr<Texture> ptr;
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		ptr = std::make_shared<GLTexture>(absPath, type);
	}
	cache.insert({absPath, ptr});
	return ptr;
}

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
