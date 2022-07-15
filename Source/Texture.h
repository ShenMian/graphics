// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Format.h"
#include "Core/Platform.h"
#include <filesystem>
#include <memory>
#include <unordered_map>
#include <vector>

class Image;

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
	 * @param path 图像文件路径.
	 * @param type 纹理类型.
	 */
	[[nodiscard]] static std::shared_ptr<Texture> create(const std::filesystem::path& path, Type type = Type::_2D);

	/**
	 * @brief 从图像创建纹理.
	 *
	 * @param image 图像.
	 * @param type  纹理类型.
	 */
	[[nodiscard]] static std::shared_ptr<Texture> create(const Image& image, Type type = Type::_2D);

	/**
	 * @brief 从图像创建立方体纹理.
	 *
	 * @param images 图像.
	 * @param type   纹理类型.
	 */
	[[nodiscard]] static std::shared_ptr<Texture> create(const std::vector<Image>& images);

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

	/**
	 * @brief 设置 R 环绕方式.
	 *
	 * @param warp 环绕方式.
	 */
	virtual void setRWarp(Warp warp) = 0;

	/**
	 * @brief 获取纹理类型.
	 */
	[[nodiscard]] Type getType() const;

	/**
	 * @brief 获取像素格式.
	 */
    [[nodiscard]] Format getFormat() const;

protected:
	Texture(Type type, Format fmt);

	Type   type;
	Format format;

	static std::unordered_map<std::filesystem::path, std::shared_ptr<Texture>> cache;
};

// TODO: 需要移动到合适的位置
#if TARGET_COMPILER == COMPILER_CLANG && __clang_major__ < 14
template <>
struct std::hash<std::filesystem::path>
{
	size_t operator()(const std::filesystem::path& path) const noexcept
	{
		return std::filesystem::hash_value(path);
	}
};
#endif
