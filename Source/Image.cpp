// Copyright 2021 SMS
// License(Apache-2.0)

#include "Image.h"
#include <format>

// #define STB_IMAGE_IMPLEMENTATION // 防止和 assimp 内的 stb 重定义
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

namespace fs = std::filesystem;

Image::Image(const std::filesystem::path& path)
{
    loadFromFile(path);
}

void Image::loadFromFile(const std::filesystem::path& path)
{
    if(!fs::exists(path) && !fs::is_regular_file(path))
        throw std::exception("no such file or directory");

    // stbi_set_flip_vertically_on_load(1);
    auto pixels = stbi_load(path.string().c_str(), &size.x, &size.y, &channels, 0);
    if(pixels == nullptr)
        throw std::exception(std::format("can't load image from file: '{}'", path.string()).c_str());

    loadFromMemory(pixels, size.x * size.y * channels, size);

    stbi_image_free(pixels);
}

void Image::loadFromMemory(const void* data, size_t dataSize, Vector2i size)
{
    this->size = size;
    this->data.resize(dataSize);
    std::memcpy(this->data.data(), data, this->data.size());
    this->data.shrink_to_fit();
}

void Image::saveToFile(const std::filesystem::path& path) const
{
	const auto ext = path.extension().string();

    if(ext == ".jpg" || ext == "jpeg")
    {
        if(!stbi_write_jpg(path.string().c_str(), size.x, size.y, channels, data.data(), 90)) // quality is between 1 and 100
            throw std::exception("can't save image to jpg/jpeg");
    }
    else if(ext == ".png")
    {
        if(!stbi_write_png(path.string().c_str(), size.x, size.y, channels, data.data(), 0))
            throw std::exception("can't save image to png");
    }
    else if(ext == ".bmp")
    {
        if(!stbi_write_bmp(path.string().c_str(), size.x, size.y, channels, data.data()))
            throw std::exception("can't save image to bmp");
    }
    else if(ext == ".tga")
    {
        if(!stbi_write_tga(path.string().c_str(), size.x, size.y, channels, data.data()))
            throw std::exception("can't save image to tga");
    }
    else
        assert(false); // 不支持的导出格式
}

uint8_t* Image::getData()
{
    return data.data();
}

const uint8_t* Image::getData() const
{
    return data.data();
}

Vector2i Image::getSize() const
{
    return size;
}

int Image::getChannelCount() const
{
    return channels;
}