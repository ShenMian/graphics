// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include <math/math.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class Shader;

/** @addtogroup shader
 *  @{
 */

/**
 * @brief 着色器程序.
 */
class Program
{
public:
	struct Descriptor;

	/**
	 * @brief 创建着色器阶段.
	 *
	 * @param desc 描述符.
	 */
	[[nodiscard]] static std::shared_ptr<Program> create(const Descriptor& desc);

	/**
	 * @brief 创建着色器阶段.
	 *
	 * @param name 着色器文件名称.
	 *
	 * 从指定位置加载具有相同名称和特定后缀的 SPIR-V 文件.
	 */
	[[nodiscard]] static std::shared_ptr<Program> create(std::string_view name);

	virtual void use() = 0;

	virtual void set_uniform(const std::string& name, int value)            = 0;
	virtual void set_uniform(const std::string& name, float value)          = 0;
	virtual void set_uniform(const std::string& name, const Vector2& value) = 0;
	virtual void set_uniform(const std::string& name, const Vector3& value) = 0;
	virtual void set_uniform(const std::string& name, const Vector4& value) = 0;
	virtual void set_uniform(const std::string& name, const Matrix4& value) = 0;

	int get_stage_count() const;

protected:
	Program(const Descriptor& desc);

	std::string name_;
	int         stage_count_ = 0;

	inline static std::unordered_map<std::string, std::shared_ptr<Program>> cache_;
};

struct Program::Descriptor
{
	std::string             name;
	std::shared_ptr<Shader> vertex;
	std::shared_ptr<Shader> fragment;
	std::shared_ptr<Shader> geometry;
	std::shared_ptr<Shader> compute;
};

/** @}*/

/**
 * @class Program
 * @details 包含若干着色器阶段.
 */
