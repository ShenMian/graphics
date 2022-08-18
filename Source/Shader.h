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

	/**
	 * @brief 将 GLSL 编译为 SPIR-V.
	 * 
	 * @param sourcePath GLSL 文件路径.
	 * @param targetPath
	 * SPIR-V 文件路径.
	 * @param stage      着色器阶段.
	 * 
	 * @warning 入口点必须为 main.
	 */
	void compile(const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath, Stage stage);

	std::string name;
	Stage       stage;
};

struct Shader::Descriptor
{
	Stage                 stage;
	std::filesystem::path path;                ///< 源码文件或 SPIR-V 文件路径.
	std::string_view      entryPoint = "main"; ///< 入口点.
};

/** @}*/
