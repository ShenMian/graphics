// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Program.h"
#include <glad/glad.h>
#include <unordered_map>

class Shader;

class GLProgram : public Program
{
public:
	GLProgram(const Descriptor& desc);
	virtual ~GLProgram();

	void use() override;

	void set_uniform(const std::string& name, int value) override;
	void set_uniform(const std::string& name, float value) override;
	void set_uniform(const std::string& name, const Vector2& value) override;
	void set_uniform(const std::string& name, const Vector3& value) override;
	void set_uniform(const std::string& name, const Vector4& value) override;
	void set_uniform(const std::string& name, const Matrix4& value) override;

	operator GLuint() const noexcept;

private:
	/**
	 * @brief 获取 uniform 缓冲块地址.
	 *
	 * @param name uniform 块名称.
	 *
	 * @return int uniform 缓冲块地址.
	 */
	int get_uniform_buffer_location(const std::string& name) const;

	/**
	 * @brief 获取 uniform 地址.
	 *
	 * @param name uniform 名称.
	 *
	 * @return int uniform 地址.
	 *
	 * @exception std::runtime_error 名称没有与着色器中处于活跃状态的 uniform 变量对应, 或名称以预留
	 *     前缀 "gl_" 开头, 或名称与原子性计数器相关联, 或存在同名的 uniform 块.
	 */
	int get_uniform_location(const std::string& name) const;

	void attach(const std::shared_ptr<Shader> shader);
	void detach(const std::shared_ptr<Shader> shader);
	void link();

	GLuint                                       handle_;
	mutable std::unordered_map<std::string, int> uniform_location_cache_;
};
