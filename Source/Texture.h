// Copyright 2021 SMS
// License(Apache-2.0)

#pragma once

#include <memory>

class Image;

class Texture
{
public:
	// 纹理过滤方式
	enum class Filter
	{
		Nearest,  // 最近点采样
		Bilinear, // 双线性过滤
		Trilinear // 三线性过滤
	};

	// 纹理环绕方式
	enum class Warp
	{
		Repeat, // 重复
		ClampToEdge,
		MirrorRepeat // 镜像重复
	};

	// 像素格式
	enum class Format
	{
		R8,
		RG8,
		RGB8,
		RGBA8,
		RGB16,
		RGBA16,
		Depth,
		Stencil,
		DepthStencil
	};

	/**
	 * @brief 从图像创建纹理.
	 *
	 * @param image 图像.
	 */
	static std::shared_ptr<Texture> create(const Image& image);

	virtual void bind(unsigned int slot = 0) = 0;

	/**
	 * @brief 设置缩小过滤方式.
	 *
	 * @param filter 过滤方式.
	 */
	virtual void setMinFilter(Filter filter) = 0;

	/**
	 * @brief 设置放大过滤方式.
	 *
	 * @param filter 过滤方式.
	 */
	virtual void setMagFilter(Filter filter) = 0;

	/**
	 * @brief 设置 S 环绕方式.
	 *
	 * @param warp 环绕方式.
	 */
	virtual void setSWarp(Warp warp) = 0;

	/**
	 * @brief 设置 T 环绕方式.
	 *
	 * @param warp 环绕方式.
	 */
	virtual void setTWarp(Warp warp) = 0;

protected:
	Format format;
};