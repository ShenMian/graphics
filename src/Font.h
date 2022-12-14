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
	Glyph getGlyph(unsigned long code, uint16_t size);

	static void init();
	static void deinit();

private:
	void     setPixelSize(uint16_t size);
	uint16_t getPixelSize() const;

	std::unordered_map<uint64_t, Glyph> glyphs;

	FT_Face handle;
};
