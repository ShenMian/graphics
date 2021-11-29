// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "../Shader.h"

class VKShader : public Shader
{
public:
	VKShader(const std::string& name, Stage stage);

	size_t getNativeHandle() const override;
};
