// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKShader.h"
#include "VKRenderer.h"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <shaderc/shaderc.hpp>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace fs = std::filesystem;

VKShader::VKShader(const Descriptor& desc) : Shader(desc)
{
	auto buf = getCode(desc.path);
	parse(buf);

	VkShaderModuleCreateInfo info = {};
	info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize                 = buf.size() * sizeof(uint32_t);
	info.pCode                    = buf.data();

	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	if(vkCreateShaderModule(renderer->getDevice(), &info, nullptr, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module");
}

VKShader::~VKShader()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	vkDestroyShaderModule(renderer->getDevice(), handle, nullptr);
}

VkShaderModule VKShader::getHandle()
{
	return handle;
}
