// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Font.h"
#include <cassert>
#include <stdexcept>

#include <ft2build.h>
#include FT_FREETYPE_H

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace fs = std::filesystem;

namespace
{

FT_Library library = nullptr;

} // namespace

Font::Font(const fs::path& path)
{
	assert(library);
	load(path);
}

Font::~Font()
{
	FT_Done_Face(handle);
}

void Font::load(const fs::path& path)
{
	if(!fs::is_regular_file(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	if(const auto err = FT_New_Face(library, path.string().c_str(), 0, &handle))
		throw std::runtime_error(fmt::format("failed to load font: {}", FT_Error_String(err)));
}

Font::Glyph Font::getGlyph(unsigned long code, uint16_t size)
{
	setPixelSize(size);
	if(const auto err = FT_Load_Char(handle, code, FT_LOAD_RENDER))
		throw std::runtime_error(fmt::format("failed to load glyph: {}", FT_Error_String(err)));

	if(auto it = glyphs.find(code); it != glyphs.end())
		return it->second;

	const auto& bitmap = handle->glyph->bitmap;
	Glyph       glyph;
	glyph.image   = Image(bitmap.buffer, bitmap.pitch * bitmap.rows, {(int)bitmap.width, (int)bitmap.rows}, 1);
	glyph.advance = {(int)handle->glyph->advance.x, (int)handle->glyph->advance.y};
	glyphs.emplace(code, glyph);

	return glyph;
}

void Font::init()
{
	if(const auto err = FT_Init_FreeType(&library))
		throw std::runtime_error(fmt::format("FreeType init failed: {}", FT_Error_String(err)));
}

void Font::deinit()
{
	FT_Done_FreeType(library);
}

void Font::setPixelSize(uint16_t size)
{
	const auto err = FT_Set_Pixel_Sizes(handle, 0, size);
	if(err == FT_Err_Invalid_Pixel_Size && !FT_IS_SCALABLE(handle))
		throw std::runtime_error("font are not scalable, can only be set to fixed sizes");
	if(err)
		throw std::runtime_error("failed to set pixel size");
}

uint16_t Font::getPixelSize() const
{
	return handle->size->metrics.x_ppem;
}
