// Copyright 2021 SMS
// License(Apache-2.0)

#include "GLTexture.h"
#include "GLRenderer.h"
#include "Core/Image.h"
#include "Format.h"
#include <cassert>
#include <unordered_map>

namespace
{

std::unordered_map<Texture::Type, GLenum> GLType = {
	{Texture::Type::_2D, GL_TEXTURE_2D},
	{Texture::Type::_3D, GL_TEXTURE_3D},
	{Texture::Type::Cube, GL_TEXTURE_CUBE_MAP},
};

std::unordered_map<Texture::Filter, GLenum> GLFilter = {
	{Texture::Filter::Nearest, GL_NEAREST},
	{Texture::Filter::Bilinear, GL_LINEAR},
	{Texture::Filter::Trilinear, GL_LINEAR_MIPMAP_LINEAR}};

std::unordered_map<Texture::Warp, GLenum> GLWarp = {
	{Texture::Warp::Repeat, GL_REPEAT},
	{Texture::Warp::MirrorRepeat, GL_MIRRORED_REPEAT},
	{Texture::Warp::ClampToEdge, GL_CLAMP_TO_EDGE}};

std::unordered_map<int, Format> ChannelsToFormat = {
	{1, Format::R8F},
	{2, Format::RG8F},
	{3, Format::RGB8F},
	{4, Format::RGBA8F},
	{6, Format::RGB16F},
	{8, Format::RGBA16F}};

uint32_t GLInternalFormat(Format fmt)
{
	switch(fmt)
	{
		using enum Format;

	case R8F:
		return GL_R8;

	case RG8F:
		return GL_RG8;

	case RGB8F:
		return GL_RGB8;

	case RGBA8F:
		return GL_RGBA8;

	case DepthStencil:
		return GL_DEPTH24_STENCIL8;

	default:
		assert(false && "no corresponding GL internal format");
		return 0;
	}
}

uint32_t GLFormat(Format fmt)
{
	switch(fmt)
	{
		using enum Format;

	case R8F:
		return GL_RED;

	case RG8F:
		return GL_RG;

	case RGB8F:
		return GL_RGB;

	case RGBA8F:
		return GL_RGBA;

	default:
		assert(false && "no corresponding GL format");
		return 0;
	}
}

}

GLTexture::GLTexture(const Image& image, Type type)
	: Texture(type, ChannelsToFormat[image.getChannelCount()])
{
	glTarget = GLType[type];

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

void GLTexture::setRWarp(Warp warp)
{
	assert(type == Type::_3D);

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

void GLTexture::bind(unsigned int slot)
{
	glBindTextureUnit(slot, handle);
	GLCheckError();
}
