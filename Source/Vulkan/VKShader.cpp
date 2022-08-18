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

// TODO: 与 GLShader 重复
namespace
{

std::unordered_map<Shader::Stage, shaderc_shader_kind> SCStage = {
    {Shader::Stage::Vertex, shaderc_glsl_vertex_shader},
    {Shader::Stage::Fragment, shaderc_glsl_fragment_shader},
    {Shader::Stage::Geometry, shaderc_glsl_geometry_shader},
    {Shader::Stage::Compute, shaderc_glsl_compute_shader}};

}

VKShader::VKShader(const Descriptor& desc) : Shader(desc)
{
	auto renderer = reinterpret_cast<VKRenderer*>(Renderer::get());

	auto path = desc.path;

	if(!fs::exists(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	if(path.extension() != ".spv")
	{
		compile(path, fs::path(path).replace_extension(".spv"), desc.stage);
		path.replace_extension(".spv");
	}

	// 读取文件内容
	const auto    fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", path));

	std::vector<char> buffer(fileSize);
	file.read(buffer.data(), fileSize);
	if(!file.good() || file.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: ", path));
	file.close();

	VkShaderModuleCreateInfo info = {};
	info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	info.codeSize                 = buffer.size();
	info.pCode                    = reinterpret_cast<const uint32_t*>(buffer.data());

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
