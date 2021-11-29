// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <memory>
#include <string>
#include <unordered_map>

/**
 * @brief 着色器程序.
 */
class Program
{
public:
	/**
	 * @brief 创建着色器程序.
	 *
	 * @param name 着色器程序名称. 将通过该名称在指定的目录中寻找对应的着色器文件并加载.
	 */
	static std::shared_ptr<Program> create(const std::string& name);

	explicit Program(const std::string& name);

	virtual void use() = 0;

	virtual void setUniform(const std::string& name, int value) = 0;
	virtual void setUniform(const std::string& name, float value) = 0;
	// virtual void setUniform(const std::string& name, const Vector2& value) = 0;
	// virtual void setUniform(const std::string& name, const Vector3& value) = 0;
	// virtual void setUniform(const std::string& name, const Vector4& value) = 0;
	// virtual void setUniform(const std::string& name, const Matrix4& value) = 0;

protected:
	std::string name;

	inline static std::unordered_map<std::string, std::shared_ptr<Program>> cache;
};
