// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Texture.h"
#include "Core/Image.h"
#include "Renderer.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <stdexcept>

#include "OpenGL/GLTexture.h"

namespace fs = std::filesystem;

namespace
{

std::unordered_map<int, Format> ChannelsToFormat = {{1, Format::R8F},    {2, Format::RG8F},   {3, Format::RGB8F},
                                                    {4, Format::RGBA8F}, {6, Format::RGB16F}, {8, Format::RGBA16F}};

}

uint32_t GetMaxMipmapLevel(const Vector2i& size)
{
	return std::log2(std::max(size.x(), size.y())) + 1;
}

std::shared_ptr<Texture> Texture::create(const fs::path& path, Format fmt, uint32_t mipmapCount, Type type)
{
	assert(type != Type::Cube);

	const auto absPath = fs::absolute(path);
	const auto it      = cache.find(absPath);
	if(it != cache.end())
		return it->second;

	std::shared_ptr<Texture> ptr;
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		ptr = std::make_shared<GLTexture>(Image(absPath), fmt, mipmapCount, type);
		break;

	case Vulkan:
		assert(false);
	}
	cache.insert({absPath, ptr});
	return ptr;
}

std::shared_ptr<Texture> Texture::create(const Image& image, Format fmt, uint32_t mipmapCount, Type type)
{
	assert(type != Type::Cube);

	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLTexture>(image, fmt, mipmapCount, type);

	case Vulkan:
		assert(false);
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

	case Vulkan:
		assert(false);
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

Texture::Texture(Type type, Format fmt) : type(type), format(fmt)
{
}
