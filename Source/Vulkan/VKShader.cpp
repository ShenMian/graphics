// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "VKShader.h"
#include "VKRenderer.h"
#include <cassert>
#include <stdexcept>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

VKShader::VKShader(const std::string& name, Stage stage)
	: Shader(name, stage)
{
	if(fs::exists(name + "." + extension[stage] + ".glsl"))
	{
		const fs::path path = name + "." + extension[stage] + ".glsl";
		const auto     size = fs::file_size(path);

		std::ifstream file(path, std::ios::binary);
		assert(file.is_open());

		// 读取源代码
		std::string buffer;
		buffer.resize(size);
		file.read(reinterpret_cast<char*>(buffer.data()), size);
		assert(file.good() && file.gcount() == size);
		buffer += "\0";

		VkShaderModuleCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		info.codeSize = buffer.size();
		info.pCode = reinterpret_cast<const uint32_t*>(buffer.data());

		auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
		auto ret = vkCreateShaderModule(renderer->getDevice(), &info, nullptr, &handle);
		if(ret != VK_SUCCESS)
			throw std::runtime_error("failed to create shader module");
	}
}

VKShader::~VKShader()
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());
	vkDestroyShaderModule(renderer->getDevice(), handle, nullptr);
}

size_t VKShader::getNativeHandle() const
{
	// TODO
	return size_t();
}
