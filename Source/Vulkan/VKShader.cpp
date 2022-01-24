// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKShader.h"
#include "VKRenderer.h"
#include <cassert>
#include <stdexcept>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

VKShader::VKShader(const Descriptor& desc)
	: Shader(desc)
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());

	const auto& path = desc.path;

	if(!fs::exists(path))
		throw std::runtime_error("file not found: " + path.string());

	// 读取文件内容
	const auto fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error("failed to open file: " + path.string());

	std::vector<char> buffer(fileSize);
	file.read(buffer.data(), fileSize);
	if(!file.good() || file.gcount() != fileSize)
		throw std::runtime_error("failed to read file: " + path.string());
	file.close();

	VkShaderModuleCreateInfo info = {};
	info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize = buffer.size();
	info.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

	if(vkCreateShaderModule(renderer->getDevice(), &info, nullptr, &handle) != VK_SUCCESS)
		throw std::runtime_error("failed to create shader module");
}

VKShader::~VKShader()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	vkDestroyShaderModule(renderer->getDevice(), handle, nullptr);
}

VkShaderModule VKShader::getNativeHandle()
{
	return handle;
}
