// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Core/Image.h"
#include <filesystem>
#include <unordered_map>

struct FT_FaceRec_;
typedef struct FT_FaceRec_* FT_Face;

class Font
{
public:
	struct Glyph
    {
        Image    image;
        Vector2i advance;
    };

	Font(const std::filesystem::path& path);
	virtual ~Font();

	/**
	 * @brief 从字体文件加载.
	 */
	void load(const std::filesystem::path& path);

	/**
	 * @brief 获取字形.
	 */
	Glyph get_glyph(unsigned long code, uint16_t size);

	static void init();
	static void deinit();

private:
	void     set_pixel_size(uint16_t size);
	uint16_t get_pixel_size() const;

	std::unordered_map<uint64_t, Glyph> glyph_cache_;

	FT_Face handle_;
};
