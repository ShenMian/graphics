// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Shader.h"
#include <vulkan/vulkan.h>

class VKShader : public Shader
{
public:
	VKShader(const Descriptor& desc);
	VKShader(const std::filesystem::path& path, Stage stage);
	VKShader(const std::string& name, Stage stage);
	virtual ~VKShader();

	VkShaderModule getNativeHandle();

private:
	VkShaderModule handle;
};
