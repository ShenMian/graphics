// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Shader.h"

class GLShader : public Shader
{
public:
	using handle_type = unsigned int;

	GLShader(const std::string& name, Stage stage);
	virtual ~GLShader();

	void load();

	size_t getNativeHandle() const override;

private:
	handle_type handle;
};
