// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include "../Texture.h"
#include <glad/glad.h>

class GLTexture : public Texture
{
public:
	GLTexture(const Image& image);
	~GLTexture();

	void bind(unsigned int slot = 0);
	void setMinFilter(Filter filter);
	void setMagFilter(Filter filter);
	void setSWarp(Warp warp);
	void setTWarp(Warp warp);
	void generateMipmap();

private:
	unsigned int handle;
	GLenum       glTarget;
};