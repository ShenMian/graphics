// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKShader.h"
#include <vulkan/vulkan.h>

VKShader::VKShader(const std::string& name, Stage stage)
	: Shader(name, stage)
{
}

size_t VKShader::getNativeHandle() const
{
	// TODO
	return size_t();
}
