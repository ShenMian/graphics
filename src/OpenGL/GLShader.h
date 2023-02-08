// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Shader.h"
#include <glad/glad.h>

class GLShader : public Shader
{
public:
	GLShader(const Descriptor& desc);
	virtual ~GLShader();

	/**
	 * @brief 获取本地句柄.
	 */
	size_t getHandle() const;

private:
	GLuint handle;
};
