// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Core/Platform.h"
#include "Format.h"
#include "math/math.hpp"
#include <cstdint>
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

// TODO: 一个想法
// create(const std::vector<Image>& image /* mipmaps */, Format fmt = Format::Unknown, Type type = Type::_2D);

class Image;

/**
 * @brief 获取纹理最大多级渐远等级.
 *
 * @param size 初始纹理大小.
 */
uint32_t GetMaxMipmapLevel(const Vector2i& size);

/**
 * @brief 纹理.
 */
class Texture
{
public:
	// 纹理过滤方式
	enum class Filter
	{
		Nearest,  ///< 最近点采样, 临近过滤.
		Bilinear, ///< 双线性过滤.
		Trilinear ///< 三线性过滤.
	};

	// 纹理环绕方式(纹理寻址方式)
	enum class Warp
	{
		Repeat,      ///< 重复.
		ClampToEdge, ///< 超出边界后重复边缘, 产生边缘被拉伸的效果.
		MirrorRepeat ///< 镜像重复, 正像/镜像交替.
	};

	// 纹理类型
	enum class Type
	{
		_2D,
		_3D,
		Cube,
	};

	/**
	 * @brief 从图像文件创建纹理.
	 *
	 * @param path        图像文件路径.
	 * @param fmt         纹理格式.
	 * @param mipmapLevel 多级渐远纹理等级.
	 * @param type        纹理类型.
	 */
	[[nodiscard]] static std::shared_ptr<Texture> create(const std::filesystem::path& path,
	                                                     Format fmt = Format::Unknown, uint32_t mipmapLevel = -1,
	                                                     Type type = Type::_2D);

	/**
	 * @brief 从图像创建纹理.
	 *
	 * @param image       图像.
	 * @param fmt         纹理格式.
	 * @param mipmapLevel 多级渐远纹理等级.
	 * @param type        纹理类型.
	 */
	[[nodiscard]] static std::shared_ptr<Texture> create(const Image& image, Format fmt = Format::Unknown,
	                                                     uint32_t mipmapLevel = -1, Type type = Type::_2D);

	/**
	 * @brief 从图像创建立方体纹理.
	 *
	 * @param images 图像.
	 * @param type   纹理类型.
	 */
	[[nodiscard]] static std::shared_ptr<Texture> create(const std::vector<Image>& images);

	virtual void bind(uint32_t binding) = 0;

	/**
	 * @brief 设置缩小过滤方式.
	 *
	 * @param filter 过滤方式, 可选的值有 Nearest, Bilinear, Trilinear.
	 */
	virtual void set_min_filter(Filter filter) = 0;

	/**
	 * @brief 设置放大过滤方式.
	 *
	 * @param filter 过滤方式, 可选的值有 Nearest, Bilinear.
	 */
	virtual void set_mag_filter(Filter filter) = 0;

	/**
	 * @brief 设置 S 环绕方式.
	 *
	 * @param warp 环绕方式.
	 */
	virtual void set_s_warp(Warp warp) = 0;

	/**
	 * @brief 设置 T 环绕方式.
	 *
	 * @param warp 环绕方式.
	 */
	virtual void set_t_warp(Warp warp) = 0;

	/**
	 * @brief 设置 R 环绕方式.
	 *
	 * @param warp 环绕方式.
	 */
	virtual void set_r_warp(Warp warp) = 0;

	/**
	 * @brief 获取纹理类型.
	 */
	[[nodiscard]] Type get_type() const;

	/**
	 * @brief 获取像素格式.
	 */
	[[nodiscard]] Format get_format() const;

protected:
	Texture(Type type, Format fmt);

	Type   type_;
	Format format_;

	/**
	 * FIXME:
	 * 1. 内存泄露.
	 * 2. 数据相同的纹理其他属性(如寻址方式)可能不同.
	 */
	inline static std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> cache_;
};
