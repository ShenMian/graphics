// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Texture.h"
#include <glad/glad.h>

class GLTexture : public Texture
{
public:
	GLTexture(const Image& image, Format fmt, uint32_t mipmapCount, Type type);
	GLTexture(const std::vector<Image>& images);
	~GLTexture();

	void bind(uint32_t binding) override;
	void setMinFilter(Filter filter) override;
	void setMagFilter(Filter filter) override;
	void setSWarp(Warp warp) override;
	void setTWarp(Warp warp) override;
	void setRWarp(Warp warp) override;
	void generateMipmap();

private:
	GLuint handle;
};
