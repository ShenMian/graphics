// Copyright 2022 ShenMian
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

Image::Image(const fs::path& path)
{
	load(path);
}

Image::Image(const void* data, size_t sizeBytes, const Vector2i& size, int channels)
{
	load(data, sizeBytes, size, channels);
}

void Image::load(const fs::path& path)
{
	if(!fs::is_regular_file(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	StbiImage pixels(stbi_load(path.string().c_str(), &size_.x, &size_.y, &channels_, 0));
	if(pixels == nullptr)
		throw std::runtime_error(fmt::format("failed load image from file: {}", path));

	load(pixels.get(), static_cast<size_t>(size_.x) * size_.y * channels_, size_, channels_);
}

void Image::load(const void* data, size_t sizeBytes, const Vector2i& size, int channels)
{
	assert(size.x * size.y * channels == sizeBytes);

	size_     = size;
	channels_ = channels;
	data_.resize(sizeBytes);
	std::memcpy(this->data_.data(), data, this->data_.size());
	data_.shrink_to_fit();
}

void Image::save(const fs::path& path) const
{
	const auto ext = path.extension().string();

	if(ext == ".jpg" || ext == "jpeg")
	{
		if(!stbi_write_jpg(path.string().c_str(), size_.x, size_.y, channels_, data_.data(),
		                   90)) // quality is between 1 and 100
			throw std::runtime_error("failed to save image to jpg/jpeg");
	}
	else if(ext == ".png")
	{
		if(!stbi_write_png(path.string().c_str(), size_.x, size_.y, channels_, data_.data(), 0))
			throw std::runtime_error("failed to save image to png");
	}
	else if(ext == ".bmp")
	{
		if(!stbi_write_bmp(path.string().c_str(), size_.x, size_.y, channels_, data_.data()))
			throw std::runtime_error("failed to save image to bmp");
	}
	else if(ext == ".tga")
	{
		if(!stbi_write_tga(path.string().c_str(), size_.x, size_.y, channels_, data_.data()))
			throw std::runtime_error("failed to save image to tga");
	}
	else
		throw std::runtime_error("unsupport format"); // 不支持的导出格式
}

void Image::setPixel(const Vector4f& color, const Vector2i& pos)
{
	assert(pos.x < size_.x && pos.y < size_.y);
	auto pixel = &data_[(pos.y * size_.x + pos.x) * channels_];
	*pixel++   = (uint8_t)(color.r * 255);
	*pixel++   = (uint8_t)(color.g * 255);
	*pixel++   = (uint8_t)(color.b * 255);
	*pixel++   = (uint8_t)(color.a * 255);
}

Vector4f Image::getPixel(const Vector2i& pos) const
{
	assert(pos.x < size_.x && pos.y < size_.y);
	const auto pixel = &data_[(pos.y * size_.x + pos.x) * channels_];
	return {pixel[0] / 255.f, pixel[1] / 255.f, pixel[2] / 255.f, pixel[3] / 255.f};
}

void Image::flipHorizontally() noexcept
{
	const auto rowSize = size_.x * channels_;

	for(size_t y = 0; y < size_.y; y++)
	{
		auto left  = data_.begin() + y * rowSize;
		auto right = data_.begin() + (y + 1) * rowSize - channels_;

		for(size_t x = 0; x < size_.x / 2; x++)
		{
			std::swap_ranges(left, left + channels_, right);
			left += channels_;
			right -= channels_;
		}
	}
}

void Image::flipVertically() noexcept
{
	const auto rowSize = size_.x * channels_;

	auto top    = data_.begin();
	auto bottom = data_.end() - rowSize;

	for(size_t y = 0; y < size_.y / 2; y++)
	{
		std::swap_ranges(top, top + rowSize, bottom);
		top += rowSize;
		bottom -= rowSize;
	}
}

uint8_t* Image::data() noexcept
{
	return data_.data();
}

const uint8_t* Image::data() const noexcept
{
	return data_.data();
}

size_t Image::sizeBytes() const noexcept
{
	return data_.size();
}

Vector2i Image::size() const noexcept
{
	return size_;
}

int Image::channelCount() const noexcept
{
	return channels_;
}
