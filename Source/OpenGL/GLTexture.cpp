// Copyright 2021 ShenMian	{
// License(Apache-2.0)

#include "GLTexture.h"
#include "Core/Image.h"
#include "Format.h"
#include <cassert>
#include <unordered_map>

namespace
{

std::unordered_map<Texture::Type, GLenum> GLType = {{Texture::Type::_2D, GL_TEXTURE_2D},
                                                    {Texture::Type::_3D, GL_TEXTURE_3D},
                                                    {Texture::Type::Cube, GL_TEXTURE_CUBE_MAP}};

std::unordered_map<Texture::Filter, GLenum> GLFilter = {{Texture::Filter::Nearest, GL_NEAREST},
                                                        {Texture::Filter::Bilinear, GL_LINEAR},
                                                        {Texture::Filter::Trilinear, GL_LINEAR_MIPMAP_LINEAR}};

std::unordered_map<Texture::Warp, GLenum> GLWarp = {{Texture::Warp::Repeat, GL_REPEAT},
                                                    {Texture::Warp::MirrorRepeat, GL_MIRRORED_REPEAT},
                                                    {Texture::Warp::ClampToEdge, GL_CLAMP_TO_EDGE}};

Format GetImageFormat(const Image& image)
{
	switch(image.channelCount())
	{
	case 1:
		return Format::R8F;

	case 2:
		return Format::RG8F;

	case 3:
		return Format::RGB8F;

	case 4:
		return Format::RGBA8F;

	case 6:
		return Format::RGB16F;

	case 8:
		return Format::RGBA16F;

	default:
		return Format::Unknown;
	}
}

// GPU
GLint GLInternalFormat(Format fmt)
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

// CPU
GLenum GLFormat(Format fmt)
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

	case RGBA_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

	case RGBA_DXT3:
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;

	case RGBA_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

	default:
		assert(false && "no corresponding GL format");
		return 0;
	}
}

GLenum GLCompressedFormat(Format fmt)
{
	switch(fmt)
	{
		using enum Format;

	case RGBA_DXT1:
		return GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;

	case RGBA_DXT3:
		return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;

	case RGBA_DXT5:
		return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;

	default:
		assert(false && "no corresponding GL format");
		return 0;
	}
}

bool IsCompress(Format fmt)
{
	switch(fmt)
	{
		using enum Format;

	case RGBA_DXT1:
	case RGBA_DXT3:
	case RGBA_DXT5:
		return true;

	default:
		return false;
	}
}

} // namespace

GLTexture::GLTexture(const Image& image, Format fmt, uint32_t mipmapCount, Type type) : Texture(type, fmt)
{
	glCreateTextures(GLType[type], 1, &handle);

	Image flippedImage = Image(image);
	flippedImage.flipVertically();

	if(format == Format::Unknown)
		format = GetImageFormat(flippedImage);

	setMinFilter(Filter::Nearest);
	setMagFilter(Filter::Bilinear);
	setSWarp(Warp::Repeat);
	setTWarp(Warp::Repeat);

	if(IsCompress(format))
	{
		glTextureParameteri(handle, GL_TEXTURE_BASE_LEVEL, 0);
		glTextureParameteri(handle, GL_TEXTURE_MAX_LEVEL, mipmapCount - 1);

		size_t blockSize;
		switch(format)
		{
		case Format::RGBA_DXT1:
			blockSize = 8;
			break;

		case Format::RGBA_DXT3:
			blockSize = 16;
			break;

		case Format::RGBA_DXT5:
			blockSize = 16;
			break;
		}

		size_t offset = 0;
		GLsizei width  = flippedImage.size().x;
		GLsizei height = flippedImage.size().y;
		for(uint32_t level = 0; level < mipmapCount; level++)
		{
			// FIXME
			size_t size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
			glCompressedTextureSubImage2D(handle, level, 0, 0, width, height, GLCompressedFormat(format), size,
			                              flippedImage.data() + offset);
			offset += size;
			width /= 2;
			height /= 2;
		}
		return;
	}

	GLsizei width  = flippedImage.size().x;
	GLsizei height = flippedImage.size().y;
	glTextureStorage2D(handle, 1, GLInternalFormat(format), width, height);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTextureSubImage2D(handle, 0, 0, 0, width, height, GLFormat(format), GL_UNSIGNED_BYTE,
			            flippedImage.data());

	if(mipmapCount == 1)
		generateMipmap();
}

GLTexture::GLTexture(const std::vector<Image>& images) : Texture(Type::Cube, GetImageFormat(images[0]))
{
	glCreateTextures(GLType[type], 1, &handle);

	setMinFilter(Filter::Nearest);
	setMagFilter(Filter::Bilinear);
	setSWarp(Warp::Repeat);
	setTWarp(Warp::Repeat);
	setRWarp(Warp::Repeat);

	for(size_t i = 0; i < images.size(); i++)
	{
		Image flippedImage = Image(images[i]);
		flippedImage.flipVertically();

		// glTextureSubImage3D(handle, 0, 0, 0, i, flippedImage.size().x, flippedImage.size().y, 1, GL_UNSIGNED_BYTE,
		//                     GLFormat(format), flippedImage.data());
		bind();
		glTexImage2D(static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i), 0, GLInternalFormat(format),
		             static_cast<GLsizei>(flippedImage.size().x), static_cast<GLsizei>(flippedImage.size().y), 0,
		             GLFormat(format), GL_UNSIGNED_BYTE, flippedImage.data());
	}
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &handle);
}

void GLTexture::setMinFilter(Filter filter)
{
	assert(filter == Filter::Nearest || filter == Filter::Bilinear || filter == Filter::Trilinear);

	glTextureParameteri(handle, GL_TEXTURE_MIN_FILTER, GLFilter[filter]);
}

void GLTexture::setMagFilter(Filter filter)
{
	assert(filter == Filter::Nearest || filter == Filter::Bilinear);

	glTextureParameteri(handle, GL_TEXTURE_MAG_FILTER, GLFilter[filter]);
}

void GLTexture::setSWarp(Warp warp)
{
	glTextureParameteri(handle, GL_TEXTURE_WRAP_S, GLWarp[warp]);
}

void GLTexture::setTWarp(Warp warp)
{
	glTextureParameteri(handle, GL_TEXTURE_WRAP_T, GLWarp[warp]);
}

void GLTexture::setRWarp(Warp warp)
{
	assert(type == Type::_3D);

	glTextureParameteri(handle, GL_TEXTURE_WRAP_R, GLWarp[warp]);
}

void GLTexture::generateMipmap()
{
	glGenerateTextureMipmap(handle);
}

void GLTexture::bind(unsigned int slot)
{
	glBindTextureUnit(slot, handle);
}
