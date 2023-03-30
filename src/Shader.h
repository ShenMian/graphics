// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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
	const std::string& get_name() const noexcept;

	/**
	 * @brief 获取入口点名称.
	 */
	const std::string& get_entry_point() const noexcept;

	/**
	 * @brief 获取阶段.
	 */
	Stage get_stage() const noexcept;

protected:
	Shader(const Descriptor& desc);
	virtual ~Shader() = default;

	/**
	 * @brief 获取 SPIR-V 代码, 若未编译则先进行编译.
	 */
	std::vector<uint32_t> get_code(std::filesystem::path path);

	/**
	 * @brief 将 GLSL 编译为 SPIR-V.
	 *
	 * @param sourcePath GLSL 文件路径.
	 * @param targetPath SPIR-V 文件路径.
	 * @param stage      着色器阶段.
	 *
	 * @warning 入口点必须为 main.
	 */
	void compile(const std::filesystem::path& sourcePath, const std::filesystem::path& targetPath, Stage stage);

	/**
	 * @brief 将 GLSL 编译为 SPIR-V.
	 *
	 * @param buf SPIR-V 代码.
	 */
	void parse(const std::vector<uint32_t>& buf);

	std::string name_;
	std::string entry_point_;
	Stage       stage_;
};

struct Shader::Descriptor
{
	Stage                 stage;
	std::filesystem::path path; ///< GLSL 源码文件或 SPIR-V 文件路径.
};

/** @}*/
