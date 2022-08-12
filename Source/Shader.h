// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

/** @addtogroup shader
 *  @{
 */

/**
 * @brief 着色器阶段.
 */
class Shader
{
public:
	/**
	 * @brief 着色器阶段.
	 */
	enum class Stage
	{
		Vertex,           ///< 顶点着色器.
		Fragment,         ///< 片段着色器.
		Pixel = Fragment, ///< 像素着色器.
		Geometry,         ///< 几何着色器.
		Compute           ///< 计算着色器.
	};

	struct Descriptor;

	/**
	 * @brief 创建着色器阶段.
	 *
	 * @param desc 描述符.
	 */
	[[nodiscard]] static std::shared_ptr<Shader> create(const Descriptor& desc);

	/**
	 * @brief 获取名称.
	 */
	const std::string& getName() const;

	/**
	 * @brief 获取阶段.
	 */
	Stage getStage() const;

protected:
	Shader(const Descriptor& desc);
	virtual ~Shader() = default;

	std::string name;
	Stage       stage;

	static std::unordered_map<Shader::Stage, const char*> extension;
};

struct Shader::Descriptor
{
	Stage                 stage;
	std::filesystem::path path;
	std::string_view      entryPoint = "main"; ///< 入口点.
};

/** @}*/
