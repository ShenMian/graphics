// Copyright 2021 SMS
// License(Apache-2.0)

#include "GLTexture.h"
#include "../Image.h"
#include "../Renderer.h"
#include <unordered_map>

namespace
{

std::unordered_map<Texture::Filter, GLenum> GLFilter = {
	{Texture::Filter::Nearest, GL_NEAREST},
	{Texture::Filter::Bilinear, GL_LINEAR},
	{Texture::Filter::Trilinear, GL_LINEAR_MIPMAP_LINEAR}};

std::unordered_map<Texture::Warp, GLenum> GLWarp = {
	{Texture::Warp::Repeat, GL_REPEAT},
	{Texture::Warp::MirrorRepeat, GL_MIRRORED_REPEAT},
	{Texture::Warp::ClampToEdge, GL_CLAMP_TO_EDGE}};

std::unordered_map<int, Texture::Format> ChannelsToFormat = {
	{1, Texture::Format::R8},
	{2, Texture::Format::RG8},
	{3, Texture::Format::RGB8},
	{4, Texture::Format::RGBA8},
	{6, Texture::Format::RGB16},
	{8, Texture::Format::RGBA16}};

uint32_t GLInternalFormat(Texture::Format fmt)
{
	switch(fmt)
	{
		using enum Texture::Format;

	case RGBA8:
		return GL_RGBA8;

	case RGB8:
		return GL_RGB8;

	case RG8:
		return GL_RG8;

	case R8:
		return GL_R8;

	case DepthStencil:
		return GL_DEPTH24_STENCIL8;

	default:
		assert(false && "no corresponding GL internal format");
		return 0;
	}
}

uint32_t GLFormat(Texture::Format fmt)
{
	switch(fmt)
	{
		using enum Texture::Format;

	case RGBA8:
		return GL_RGBA;

	case RGB8:
		return GL_RGB;

	case RG8:
		return GL_RG;

	case R8:
		return GL_RED;

	default:
		assert(false && "no corresponding GL format");
		return 0;
	}
}

}

GLTexture::GLTexture(const Image& image)
{
	format = ChannelsToFormat[image.getChannelCount()];

	glCreateTextures(glTarget, 1, &handle);
	bind();

	setMinFilter(Filter::Nearest);
	setMagFilter(Filter::Bilinear);
	setSWarp(Warp::Repeat);
	setTWarp(Warp::Repeat);

	glTexImage2D(glTarget, 0, GLInternalFormat(format), image.getSize().x, image.getSize().y, 0, GLFormat(format), GL_UNSIGNED_BYTE, image.getData());
	GLCheckError();
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &handle);
}

void GLTexture::setMinFilter(Filter filter)
{
	assert(filter == Filter::Nearest || filter == Filter::Bilinear || filter == Filter::Trilinear);

	bind();
	glTexParameteri(glTarget, GL_TEXTURE_MIN_FILTER, GLFilter[filter]);
	GLCheckError();
}

void GLTexture::setMagFilter(Filter filter)
{
	assert(filter == Filter::Nearest || filter == Filter::Bilinear);

	bind();
	glTexParameteri(glTarget, GL_TEXTURE_MAG_FILTER, GLFilter[filter]);
	GLCheckError();
}

void GLTexture::setSWarp(Warp warp)
{
	bind();
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_S, GLWarp[warp]);
	GLCheckError();
}

void GLTexture::setTWarp(Warp warp)
{
	bind();
	glTexParameteri(glTarget, GL_TEXTURE_WRAP_T, GLWarp[warp]);
	GLCheckError();
}

void GLTexture::generateMipmap()
{
	bind();
	glGenerateMipmap(glTarget);
	GLCheckError();
}

void GLTexture::bind(unsigned int slot) {
	glBindTextureUnit(slot, handle);
	GLCheckError();
}
