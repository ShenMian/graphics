// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Shader.h"
#include <glad/glad.h>

class GLShader : public Shader
{
public:
	GLShader(const Descriptor& desc);
	GLShader(const std::filesystem::path& path, Stage stage);
	GLShader(const std::string& name, Stage stage);
	virtual ~GLShader();

	/**
	 * @brief 获取本地句柄.
	 */
	size_t getNativeHandle() const;

private:
	void load();

	GLuint handle;
};
