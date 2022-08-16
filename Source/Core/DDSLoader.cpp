// Copyright 2021 ShenMian
// License(Apache-2.0)

#include <cassert>
#include <filesystem>
#include <fstream>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

constexpr uint32_t dds_magic_number = 0x20534444; // "DDS "

struct DDSPixelFormat
{
	[[maybe_unused]] uint32_t size;
	[[maybe_unused]] uint32_t flags;
	[[maybe_unused]] uint32_t fourCC;
	[[maybe_unused]] uint32_t RGBBitCount;
	[[maybe_unused]] uint32_t RBitMask;
	[[maybe_unused]] uint32_t GBitMask;
	[[maybe_unused]] uint32_t BBitMask;
	[[maybe_unused]] uint32_t ABitMask;
};

struct DDSHeader
{
	[[maybe_unused]] uint32_t       size;
	[[maybe_unused]] uint32_t       flags;
	[[maybe_unused]] uint32_t       height;
	[[maybe_unused]] uint32_t       width;
	[[maybe_unused]] uint32_t       pitchOrLinearSize;
	[[maybe_unused]] uint32_t       depth; // only if DDS_HEADER_FLAGS_VOLUME is set in flags
	[[maybe_unused]] uint32_t       mipMapCount;
	[[maybe_unused]] uint32_t       reserved1[11];
	[[maybe_unused]] DDSPixelFormat ddspf;
	[[maybe_unused]] uint32_t       caps;
	[[maybe_unused]] uint32_t       caps2;
	[[maybe_unused]] uint32_t       caps3;
	[[maybe_unused]] uint32_t       caps4;
	[[maybe_unused]] uint32_t       reserved2;
};

struct DDSHeaderDXT10
{
	[[maybe_unused]] uint32_t dxgiFormat;
	[[maybe_unused]] uint32_t dimension;
	[[maybe_unused]] uint32_t miscFlag; // see D3D11_RESOURCE_MISC_FLAG
	[[maybe_unused]] uint32_t arraySize;
	[[maybe_unused]] uint32_t miscFlags2;
};

namespace DDSFlag
{

enum
{
	Alpha      = 0x00000001,
	Compressed = 0x00000004,
	Cubemap    = 0x00000200,
	Mipmaps    = 0x00020000,
	Depth      = 0x00800000
};

} // namespace DDSFlag

template <typename T>
void Read(std::istream& stream, T& value)
{
	static_assert(!std::is_pointer<T>::value);
	stream.read(reinterpret_cast<char*>(&value), sizeof(T));
	assert(stream.good() && stream.gcount() == sizeof(T));
}

template <typename T>
T Read(std::istream& stream)
{
	T value;
	Read(stream, value);
	return value;
}

namespace fs = std::filesystem;

void LoadDDS(const fs::path& path)
{
	const auto    fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: '{}'", path));

	if(fileSize < sizeof(uint32_t) + sizeof(DDSHeader))
		throw std::runtime_error("file size too small");

	if(Read<uint32_t>(file) != dds_magic_number)
		throw std::runtime_error("invalid magic number");

	DDSHeader header;
	Read(file, header);

	const bool hasDepth  = header.flags & DDSFlag::Depth;
	const bool isCubeMap = header.flags & DDSFlag::Cubemap;

	const auto MakeFourCC = [](const char* str) {
		assert(str[4] == '\0');
		return *reinterpret_cast<const std::uint32_t*>(str);
	};

	if(isCubeMap) // Cube
	{
	}
	else if(hasDepth) // 3D
	{
	}
	else // 2D
	{
	}
}
