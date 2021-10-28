// Copyright 2021 SMS
// License(Apache-2.0)

#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace fs = std::filesystem;

Image::Image(const std::filesystem::path& path)
{
    if(!fs::exists(path) && !fs::is_regular_file(path))
        throw std::exception("no such file or directory");

    // stbi_set_flip_vertically_on_load(1);
    data = stbi_load(path.string().c_str(), &size.x, &size.y, &channels, 0);
    if(data == nullptr)
        throw std::exception(std::format("can't load image from file: '{}'", path.string()).c_str());
}

Image::~Image()
{
    stbi_image_free(data);
}

unsigned char* Image::getData()
{
    return data;
}

unsigned char* Image::getData() const
{
    return data;
}

Vector2i Image::getSize() const
{
    return size;
}

int Image::getChannelCount() const
{
    return channels;
}