// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <Math/Math.hpp>
#include <cstdint>
#include <filesystem>
#include <vector>

/** @addtogroup core
 *  @{
 */

 /**
  * @brief 图像.
  */
class Image
{
public:
	/**
	 * @brief 默认构造函数.
	 */
	Image() = default;

	/**
	 * @brief 构造函数, 从文件载入图像.
	 *
	 * @param path 图像文件地址.
	 */
	Image(const std::filesystem::path& path);

	/**
	 * @brief 构造函数, 从内存载入图像.
	 *
	 * @param data      图像内存地址.
	 * @param sizeBytes 图像数据大小, 单位: 字节.
	 * @param size      图像尺寸.
	 * @param channels  通道数.
	 */
	Image(const void* data, size_t sizeBytes, Vector2i size, int channels);

	/**
	 * @brief 从文件载入图像.
	 *
	 * @param path 图像文件地址.
	 */
	void loadFromFile(const std::filesystem::path& path);

	/**
	 * @brief 从内存载入图像.
	 *
	 * @param data      图像内存地址.
	 * @param sizeBytes 图像数据大小, 单位: 字节.
	 * @param size      图像尺寸.
	 * @param channels  通道数.
	 */
	void loadFromMemory(const void* data, size_t sizeBytes, Vector2i size, int channels);

	/**
	 * @brief 导出图像到文件.
	 *
	 * @param path 图像文件地址.
	 *
	 * @note 保存 jpg 格式文件时使用的质量为 90.
	 */
	void saveToFile(const std::filesystem::path& path) const;

	/**
	 * @brief 获取图像数据.
	 */
	void setPixel(Vector4f color, Size2 pos);

	/**
	 * @brief 获取指定像素.
	 */
	Vector4f getPixel(Size2 pos) const;

	/**
	 * @brief 水平(左右)翻转.
	 */
	void flipHorizontally() noexcept;

	/**
	 * @brief 垂直(上下)翻转.
	 */
	void flipVertically() noexcept;

	/**
	 * @brief 获取图像数据.
	 */
	uint8_t* getData() noexcept;

	/**
	 * @brief 获取图像数据.
	 */
	const uint8_t* getData() const noexcept;

	/**
	 * @brief 获取图像数据大小.
	 */
	size_t getDataSize() const noexcept;

	/**
	 * @brief 获取图像分辨率.
	 */
	Vector2i getSize() const noexcept;

	/**
	 * @brief 获取通道数.
	 */
	int getChannelCount() const noexcept;

private:
	std::vector<uint8_t> data;
	Vector2i             size;
	int                  channels = 0;
};

/** @}*/
