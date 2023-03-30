// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Shader.h"
#include <vulkan/vulkan.h>

class VKShader : public Shader
{
public:
	VKShader(const Descriptor& desc);
	virtual ~VKShader();

	VkShaderModule get_handle();

private:
	VkShaderModule handle_;
};
