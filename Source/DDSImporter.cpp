// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "DDSImporter.h"
#include <fstream>
#include <stdexcept>
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace fs = std::filesystem;

namespace
{

template <typename T>
T Read(std::ifstream& file)
{
	T buf;
	file.read(reinterpret_cast<char*>(&buf), sizeof(T));
	return buf;
}

enum
{
	FLAGS_Alpha      = 0x00000001,
	FLAGS_Compressed = 0x00000004,
	FLAGS_Cubemap    = 0x00000200,
	FLAGS_Mipmaps    = 0x00020000,
	FLAGS_Depth      = 0x00800000,

	CAPS2_Cubemap_PositiveX = 0x400,
	CAPS2_Cubemap_NegativeX = 0x800,
	CAPS2_Cubemap_PositiveY = 0x1000,
	CAPS2_Cubemap_NegativeY = 0x2000,
	CAPS2_Cubemap_PositiveZ = 0x4000,
	CAPS2_Cubemap_NegativeZ = 0x8000
};

struct DDSPixelFormat
{
	uint32_t size;
	uint32_t flags;
	uint32_t fourCC;
	uint32_t RGBBitCount;
	uint32_t RBitMask;
	uint32_t GBitMask;
	uint32_t BBitMask;
	uint32_t ABitMask;
};

struct DDSHeader
{
	uint32_t       size;
	uint32_t       flags;
	uint32_t       height;
	uint32_t       width;
	uint32_t       pitchOrLinearSize;
	uint32_t       depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
	uint32_t       mipMapCount;
	uint32_t       reserved1[11];
	DDSPixelFormat format;
	uint32_t       caps;
	uint32_t       caps2;
	uint32_t       caps3;
	uint32_t       caps4;
	uint32_t       reserved2;
};

struct DDSHeaderDXT10
{
	uint32_t dxgiFormat;
	uint32_t dimension;
	uint32_t miscFlag; // see D3D11_RESOURCE_MISC_FLAG
	uint32_t arraySize;
	uint32_t miscFlags2;
};

} // namespace

std::shared_ptr<Texture> DDSImporter::load(const fs::path& path)
{
	if(!fs::exists(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", path));

	auto magicNumber = Read<uint32_t>(file);
	if(memcmp(&magicNumber, "DDS ", 4))
		throw std::runtime_error("invalid format");

	auto header = Read<DDSHeader>(file);

	bool compressed = true;

	size_t blockSize;
	Format fmt;
	if(memcmp("DXT1", &header.format.fourCC, 4))
	{
		// BC1 compression (DXT1)
		fmt = Format::RGBA_DXT1;
		blockSize = 8;
	}
	else if(memcmp("DXT2", &header.format.fourCC, 4) || memcmp("DXT3", &header.format.fourCC, 4))
	{
		// BC2 compression (DXT2 or DXT3)
		fmt = Format::RGBA_DXT3;
		blockSize = 16;
	}
	else if(memcmp("DXT4", &header.format.fourCC, 4) || memcmp("DXT5", &header.format.fourCC, 4))
	{
		// BC3 compression (DXT4 or DXT5)
		fmt = Format::RGBA_DXT5;
		blockSize = 16;
	}
	else if(memcmp("ETC1", &header.format.fourCC, 4))
	{
		blockSize = 8;
		assert(false);
	}
	else if(memcmp("DX10", &header.format.fourCC, 4))
	{
		const auto headerDX10 = Read<DDSHeaderDXT10>(file);
		assert(false);
	}
	else if(header.format.RGBBitCount == 32 && header.format.RBitMask & 0xff0000 && header.format.GBitMask & 0xff00 &&
	        header.format.BBitMask & 0xff && header.format.ABitMask & 0xff000000)
	{
		compressed = false;
	}
	else
		throw std::runtime_error(fmt::format("unsupported FourCC code"));

	const bool hasDepth  = header.flags & FLAGS_Depth;
	const bool isCubeMap = header.flags & FLAGS_Cubemap;

	Texture::Type type;
	if(isCubeMap)
		type = Texture::Type::Cube;
	else if(hasDepth)
		type = Texture::Type::_3D;
	else
		type = Texture::Type::_2D;

	if(isCubeMap)
	{
		if(!(header.caps2 & CAPS2_Cubemap_PositiveX && header.caps2 & CAPS2_Cubemap_NegativeX &&
		     header.caps2 & CAPS2_Cubemap_PositiveY && header.caps2 & CAPS2_Cubemap_NegativeY &&
		     header.caps2 & CAPS2_Cubemap_PositiveZ && header.caps2 & CAPS2_Cubemap_NegativeZ))
			throw std::runtime_error("cubemap incomplete");
	}

	size_t size   = 0;
	auto   width  = header.width;
	auto   height = header.height;
	auto   depth  = std::max(header.depth, 1u);

	size += width * height * depth * 4;
	for(uint32_t level = 0; level < header.mipMapCount; ++level)
	{
		size += width * height * depth;
		width  = std::max(width / 2, 1u);
		height = std::max(height / 2, 1u);
		depth  = std::max(depth / 2, 1u);
	}

	std::vector<char> buf(size);
	file.read(buf.data(), size);

	return Texture::create(Image(buf.data(), size, Vector2i(header.width, header.height), 4), fmt, header.mipMapCount,
	                       type);
}
