// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include <unordered_map>
#include <glad/glad.h>

class Shader;

class GLProgram : public Program
{
public:
	GLProgram(const Descriptor& desc);
	virtual ~GLProgram();

	void use() override;

	void setUniform(const std::string& name, int value) override;
	void setUniform(const std::string& name, float value) override;
	void setUniform(const std::string& name, const Vector2& value) override;
	void setUniform(const std::string& name, const Vector3& value) override;
	void setUniform(const std::string& name, const Vector4& value) override;
	void setUniform(const std::string& name, const Matrix4& value) override;

	operator GLuint() const;

private:
	/**
	 * @brief 获取 uniform 缓冲块地址.
	 *
	 * @param name uniform 块名称.
	 * @return int uniform 缓冲块地址.
	 */
	int getUniformBufferLocation(const std::string& name);

	/**
	 * @brief 获取 uniform 地址.
	 *
	 * @param name uniform 名称.
	 * @return int uniform 地址.
	 * @exception std::runtime_error 名称没有与着色器中处于活跃状态的 uniform 变量对应, 或名称以预留
	 *     前缀 "gl_" 开头, 或名称与原子性计数器相关联, 或存在同名的 uniform 块.
	 */
	int getUniformLocation(const std::string& name);

	/**
	 * @brief 附加着色器阶段到着色器程序.
	 *
	 * @param shader 着色器阶段.
	 */
	void attach(const std::shared_ptr<Shader> shader);

	/**
	 * @brief 链接着色器程序.
	 */
	void link();

	GLuint                               handle;
	std::unordered_map<std::string, int> uniformLocations;
};
