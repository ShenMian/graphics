// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Shader.h"
#include <glad/glad.h>

class GLShader : public Shader
{
public:
	GLShader(const std::string& name, Stage stage);
	virtual ~GLShader();

	void load();

	/**
	 * @brief 获取本地句柄.
	 */
	size_t getNativeHandle() const;

private:
	GLuint handle;
};
