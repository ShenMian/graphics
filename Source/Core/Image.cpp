// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Image.h"
#include <cstring>
#include <memory>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

#ifndef STB_IMAGE_IMPLEMENTATION // 防止和其他第三方库内的 stb 重定义
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb_image_write.h>

namespace fs = std::filesystem;

using StbiImage = std::unique_ptr<unsigned char, decltype([](auto data) { stbi_image_free(data); })>;

Image::Image(const std::filesystem::path& path)
{
	loadFromFile(path);
}

Image::Image(const void* data, size_t sizeBytes, Vector2i size, int channels)
{
	loadFromMemory(data, sizeBytes, size, channels);
}

void Image::loadFromFile(const std::filesystem::path& path)
{
	if(!fs::is_regular_file(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	StbiImage pixels(stbi_load(path.string().c_str(), &size.x, &size.y, &channels, 0));
	if(pixels == nullptr)
		throw std::runtime_error(fmt::format("failed load image from file: {}", path));

	loadFromMemory(pixels.get(), static_cast<size_t>(size.x) * size.y * channels, size, channels);
}

void Image::loadFromMemory(const void* data, size_t sizeBytes, Vector2i size, int channels)
{
	this->size     = size;
	this->channels = channels;
	this->data.resize(sizeBytes);
	std::memcpy(this->data.data(), data, this->data.size());
	this->data.shrink_to_fit();
}

void Image::saveToFile(const std::filesystem::path& path) const
{
	const auto ext = path.extension().string();

	if(ext == ".jpg" || ext == "jpeg")
	{
		if(!stbi_write_jpg(path.string().c_str(), size.x, size.y, channels, data.data(),
		                   90)) // quality is between 1 and 100
			throw std::runtime_error("failed to save image to jpg/jpeg");
	}
	else if(ext == ".png")
	{
		if(!stbi_write_png(path.string().c_str(), size.x, size.y, channels, data.data(), 0))
			throw std::runtime_error("failed to save image to png");
	}
	else if(ext == ".bmp")
	{
		if(!stbi_write_bmp(path.string().c_str(), size.x, size.y, channels, data.data()))
			throw std::runtime_error("failed to save image to bmp");
	}
	else if(ext == ".tga")
	{
		if(!stbi_write_tga(path.string().c_str(), size.x, size.y, channels, data.data()))
			throw std::runtime_error("failed to save image to tga");
	}
	else
		assert(false); // 不支持的导出格式
}

void Image::setPixel(Vector4f color, Size2 pos)
{
	assert(pos.x < size.x && pos.y < size.y);
	auto pixel = &data[(pos.y * size.x + pos.x) * channels];
	*pixel++   = (uint8_t)(color.r * 255);
	*pixel++   = (uint8_t)(color.g * 255);
	*pixel++   = (uint8_t)(color.b * 255);
	*pixel++   = (uint8_t)(color.a * 255);
}

Vector4f Image::getPixel(Size2 pos) const
{
	assert(pos.x < size.x && pos.y < size.y);
	const auto pixel = &data[(pos.y * size.x + pos.x) * channels];
	return {pixel[0] / 255.f, pixel[1] / 255.f, pixel[2] / 255.f, pixel[3] / 255.f};
}

void Image::flipHorizontally() noexcept
{
	const auto rowSize = size.x * channels;

	for(size_t y = 0; y < size.y; y++)
	{
		auto left  = data.begin() + y * rowSize;
		auto right = data.begin() + (y + 1) * rowSize - channels;

		for(size_t x = 0; x < size.x / 2; x++)
		{
			std::swap_ranges(left, left + channels, right);
			left += channels;
			right -= channels;
		}
	}
}

void Image::flipVertically() noexcept
{
	const auto rowSize = size.x * channels;

	auto top    = data.begin();
	auto bottom = data.end() - rowSize;

	for(size_t y = 0; y < size.y / 2; y++)
	{
		std::swap_ranges(top, top + rowSize, bottom);
		top += rowSize;
		bottom -= rowSize;
	}
}

uint8_t* Image::getData() noexcept
{
	return data.data();
}

const uint8_t* Image::getData() const noexcept
{
	return data.data();
}

size_t Image::getDataSize() const noexcept
{
	return data.size();
}

Vector2i Image::getSize() const noexcept
{
	return size;
}

int Image::getChannelCount() const noexcept
{
	return channels;
}
