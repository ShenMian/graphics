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

	// TODO: 与 GLShader 重复
	if(path.extension() == ".glsl")
		compile(path, desc.stage);
	if(path.extension() != ".spv")
		path.replace_extension(".spv");

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

// TODO: 与 GLShader 重复
void VKShader::compile(const std::filesystem::path& sourcePath, Stage stage)
{
	const auto targetPath = fs::path(sourcePath).replace_extension(".spv");
	if(fs::exists(targetPath) && fs::last_write_time(sourcePath) < fs::last_write_time(targetPath))
		return;

	// 读取源代码
	const auto    fileSize = fs::file_size(sourcePath);
	std::ifstream sourceFile(sourcePath, std::ios::binary);
	if(!sourceFile.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", sourcePath));

	std::vector<char> buffer(fileSize);
	sourceFile.read(buffer.data(), fileSize);
	if(!sourceFile.good() || sourceFile.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: {}", sourcePath));
	sourceFile.close();

	const std::string source(buffer.begin(), buffer.end());

	// 将 GLSL 编译为 SPIR-V
	shaderc::Compiler       compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);
	auto res = compiler.CompileGlslToSpv(source, SCStage[stage], sourcePath.stem().string().c_str(), options);
	if(res.GetCompilationStatus() != shaderc_compilation_status_success)
		throw std::runtime_error(fmt::format("failed to compile shader: {}", res.GetErrorMessage()));
	const std::vector<uint32_t> spv(res.cbegin(), res.cend());

	// 写入编译结果
	std::ofstream targetFile(targetPath, std::ios::binary);
	if(!targetFile.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", sourcePath));
	targetFile.write((const char*)spv.data(), spv.size() * sizeof(uint32_t));
	targetFile.close();
}

VkShaderModule VKShader::getNativeHandle()
{
	return handle;
}
