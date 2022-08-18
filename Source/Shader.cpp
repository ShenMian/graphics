// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Shader.h"
#include "Renderer.h"
#include <cassert>
#include <fstream>
#include <shaderc/shaderc.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

#include "OpenGL/GLShader.h"
#include "Vulkan/VKShader.h"

namespace fs = std::filesystem;

namespace
{

std::unordered_map<Shader::Stage, shaderc_shader_kind> SCStage = {
    {Shader::Stage::Vertex, shaderc_glsl_vertex_shader},
    {Shader::Stage::Fragment, shaderc_glsl_fragment_shader},
    {Shader::Stage::Geometry, shaderc_glsl_geometry_shader},
    {Shader::Stage::Compute, shaderc_glsl_compute_shader}};

}

std::shared_ptr<Shader> Shader::create(const Descriptor& desc)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLShader>(desc);

	case Vulkan:
		return std::make_shared<VKShader>(desc);

	default:
		assert(false);
	}
	return nullptr;
}

const std::string& Shader::getName() const
{
	return name;
}

Shader::Stage Shader::getStage() const
{
	return stage;
}

Shader::Shader(const Descriptor& desc) : name(desc.path.filename().string()), stage(desc.stage)
{
}

/**
 * TODO:
 * - 支持定义宏.
 * - 设置优化选项.
 */
void Shader::compile(const fs::path& sourcePath, const fs::path& targetPath, Stage stage)
{
	// 若目标文件比源码文件新, 跳过编译
	if(fs::exists(targetPath) && fs::last_write_time(sourcePath) < fs::last_write_time(targetPath))
		return;

	// 读取源代码
	std::ifstream file(sourcePath, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", sourcePath));

	std::stringstream buf;
	buf << file.rdbuf();
	std::string source = buf.str();

	file.close();

	// 编译
	shaderc::CompileOptions options;
	if(Renderer::getAPI() == Renderer::API::OpenGL)
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
	else if(Renderer::getAPI() == Renderer::API::Vulkan)
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	else
		throw std::runtime_error("unsupported shader language (only support GLSL)");
	options.SetOptimizationLevel(shaderc_optimization_level_zero);

	shaderc::Compiler compiler;
	const auto res = compiler.CompileGlslToSpv(source, SCStage[stage], sourcePath.stem().string().c_str(), options);
	if(res.GetCompilationStatus() != shaderc_compilation_status_success)
		throw std::runtime_error(fmt::format("failed to compile shader: {}", res.GetErrorMessage()));
	const std::vector<uint32_t> spirv(res.cbegin(), res.cend());

	// 写入二进制代码
	std::ofstream targetFile(targetPath, std::ios::binary);
	if(!targetFile.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", targetPath));
	targetFile.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
	targetFile.close();
}
