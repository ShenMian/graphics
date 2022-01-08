// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Texture.h"
#include <glad/glad.h>

class GLTexture : public Texture
{
public:
	GLTexture(const Image& image, Type type);
	GLTexture(const std::vector<Image>& images);
	~GLTexture();

	void bind(unsigned int slot = 0) override;
	void setMinFilter(Filter filter) override;
	void setMagFilter(Filter filter) override;
	void setSWarp(Warp warp) override;
	void setTWarp(Warp warp) override;
	void setRWarp(Warp warp) override;
	void generateMipmap();

private:
	GLuint handle;
	GLenum glTarget;
};
