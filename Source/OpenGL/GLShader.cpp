// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLShader.h"
#include <cassert>
#include <fstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

namespace fs = std::filesystem;

namespace
{

std::unordered_map<Shader::Stage, GLenum> GLStage = {{Shader::Stage::Vertex, GL_VERTEX_SHADER},
                                                     {Shader::Stage::Fragment, GL_FRAGMENT_SHADER},
                                                     {Shader::Stage::Geometry, GL_GEOMETRY_SHADER},
                                                     {Shader::Stage::Compute, GL_COMPUTE_SHADER}};

std::unordered_map<Shader::Stage, shaderc_shader_kind> SCStage = {
    {Shader::Stage::Vertex, shaderc_glsl_vertex_shader},
    {Shader::Stage::Fragment, shaderc_glsl_fragment_shader},
    {Shader::Stage::Geometry, shaderc_glsl_geometry_shader},
    {Shader::Stage::Compute, shaderc_glsl_compute_shader}};

} // namespace

GLShader::GLShader(const Descriptor& desc) : Shader(desc), handle(glCreateShader(GLStage[stage]))
{
	auto path = desc.path;

	if(!fs::exists(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

#if 1
	if(path.extension() == ".glsl")
		compile(path, desc.stage);
	if(path.extension() != ".spv")
		path.replace_extension(".spv");

	const auto    fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: '{}'", path));

	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
	if(!file.good() || file.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: '{}'", path));
	file.close();

	glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(),
	               (GLsizei)buffer.size() * sizeof(uint32_t));
	glSpecializeShader(handle, "main", 0, nullptr, nullptr); // 指定入口点函数名称

	{
		spirv_cross::Compiler compiler(buffer);
		const auto            res = compiler.get_shader_resources();
		if(res.uniform_buffers.empty())
			return;

		puts("uniform buffers");
		for(const auto& buf : res.uniform_buffers)
		{
			const auto& type        = compiler.get_type(buf.base_type_id);
			const auto  size        = compiler.get_declared_struct_size(type);
			const auto  binding     = compiler.get_decoration(buf.id, spv::DecorationBinding);
			const auto  memberCount = type.member_types.size();

			puts(fmt::format("|-{}\n"
			                 "  |-binding: {}\n"
			                 "  |-size   : {} bytes\n"
			                 "  `-members: {}",
			                 buf.name, binding, size, memberCount)
			         .c_str());

			const auto offset = compiler.type_struct_member_offset(type, 1);
		}
	}
#else
	// 读取文件内容
	const auto    fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: '{}'", path));

	std::vector<char> buffer(fileSize);
	file.read((char*)buffer.data(), fileSize);
	if(!file.good() || file.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: '{}'", path));
	file.close();

	buffer.push_back('\0');
	const auto source = buffer.data();
	glShaderSource(handle, 1, &source, nullptr);
	glCompileShader(handle);

	// 获取编译结果状态
	int status;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	if(!status)
	{
		// 获取报错内容
		int size;
		glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);
		std::string info(size, '\0');
		glGetShaderInfoLog(handle, (GLsizei)info.size(), &size, info.data());
		throw std::runtime_error(fmt::format("shader '{}' compile error: {}", desc.path.filename().string(), info));
	}
#endif
}

GLShader::~GLShader()
{
	glDeleteShader(handle);
}

void GLShader::compile(const fs::path& sourcePath, Stage stage)
{
	const auto targetPath = fs::path(sourcePath).replace_extension(".spv");
	if(fs::exists(targetPath) && fs::last_write_time(sourcePath) < fs::last_write_time(targetPath))
		return;

	// 读取源代码
	const auto    fileSize = fs::file_size(sourcePath);
	std::ifstream sourceFile(sourcePath, std::ios::binary);
	if(!sourceFile.is_open())
		throw std::runtime_error(fmt::format("failed to open file: '{}'", sourcePath));

	std::vector<char> buffer(fileSize);
	sourceFile.read(buffer.data(), fileSize);
	if(!sourceFile.good() || sourceFile.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: '{}'", sourcePath));
	sourceFile.close();

	const std::string source(buffer.begin(), buffer.end());

	// 将 GLSL 编译为 SPIR-V
	shaderc::Compiler       compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
	// options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);
	auto res = compiler.CompileGlslToSpv(source, SCStage[stage], sourcePath.stem().string().c_str(), options);
	if(res.GetCompilationStatus() != shaderc_compilation_status_success)
		throw std::runtime_error(fmt::format("failed to compile shader: {}", res.GetErrorMessage()));
	const std::vector<uint32_t> spv(res.cbegin(), res.cend());

	// 写入编译结果
	std::ofstream targetFile(targetPath, std::ios::binary);
	if(!targetFile.is_open())
		throw std::runtime_error(fmt::format("failed to open file: '{}'", sourcePath));
	targetFile.write(reinterpret_cast<const char*>(spv.data()), spv.size() * sizeof(uint32_t));
	targetFile.close();
}

size_t GLShader::getNativeHandle() const
{
	return handle;
}
