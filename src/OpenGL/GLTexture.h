// Copyright 2022 ShenMian
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
	void set_min_filter(Filter filter) override;
	void set_mag_filter(Filter filter) override;
	void set_s_warp(Warp warp) override;
	void set_t_warp(Warp warp) override;
	void set_r_warp(Warp warp) override;
	void generate_mipmap();

private:
	GLuint handle;
};
