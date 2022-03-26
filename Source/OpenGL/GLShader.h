// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Shader.h"
#include <glad/glad.h>

class GLShader : public Shader
{
public:
	GLShader(const Descriptor& desc);
	virtual ~GLShader();

	void compile(const std::filesystem::path& path, Stage stage);

	/**
	 * @brief 获取本地句柄.
	 */
	size_t getNativeHandle() const;

private:
	GLuint handle;
};
