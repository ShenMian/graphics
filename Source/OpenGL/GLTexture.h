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

	void bind(unsigned int slot = 0) override;
	void setMinFilter(Filter filter) override;
	void setMagFilter(Filter filter) override;
	void setSWarp(Warp warp) override;
	void setTWarp(Warp warp) override;
	void generateMipmap();

private:
	unsigned int handle;
	GLenum       glTarget;
};