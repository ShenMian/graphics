// Copyright 2022 ShenMian
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
	FT_Done_Face(handle_);
}

void Font::load(const fs::path& path)
{
	if(!fs::is_regular_file(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	if(const auto err = FT_New_Face(library, path.string().c_str(), 0, &handle_))
		throw std::runtime_error(fmt::format("failed to load font: {}", FT_Error_String(err)));
}

Font::Glyph Font::get_glyph(unsigned long code, uint16_t size)
{
	if(auto it = glyph_cache_.find(code); it != glyph_cache_.end())
		return it->second;

	set_pixel_size(size);
	if(const auto err = FT_Load_Char(handle_, code, FT_LOAD_RENDER))
		throw std::runtime_error(fmt::format("failed to load glyph: {}", FT_Error_String(err)));

	const auto& bitmap = handle_->glyph->bitmap;
	Glyph       glyph;
	glyph.image   = Image(bitmap.buffer, bitmap.pitch * bitmap.rows, {(int)bitmap.width, (int)bitmap.rows}, 1);
	glyph.advance = {(int)handle_->glyph->advance.x, (int)handle_->glyph->advance.y};
	glyph_cache_.emplace(code, glyph);

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

void Font::set_pixel_size(uint16_t size)
{
	const auto err = FT_Set_Pixel_Sizes(handle_, 0, size);
	if(err == FT_Err_Invalid_Pixel_Size && !FT_IS_SCALABLE(handle_))
		throw std::runtime_error("font are not scalable, can only be set to fixed sizes");
	if(err)
		throw std::runtime_error("failed to set pixel size");
}

uint16_t Font::get_pixel_size() const
{
	return handle_->size->metrics.x_ppem;
}
