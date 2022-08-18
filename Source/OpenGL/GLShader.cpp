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

	if(path.extension() != ".spv")
	{
		compile(path, fs::path(path).replace_extension(".spv"), desc.stage);
		path.replace_extension(".spv");
	}

	const auto    fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error(fmt::format("failed to open file: {}", path));

	std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(buffer.data()), fileSize);
	if(!file.good() || file.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: {}", path));
	file.close();

	spirv_cross::Compiler compiler(buffer);
	assert(compiler.get_entry_points_and_stages().size() == 1);
	std::string entryPointName = compiler.get_entry_points_and_stages().front().name;

	glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(),
	               (GLsizei)buffer.size() * sizeof(uint32_t));
	glSpecializeShader(handle, entryPointName.c_str(), 0, nullptr, nullptr);

	{
		const auto res = compiler.get_shader_resources();
		if(res.uniform_buffers.empty())
			return;

		puts("Uniform buffers");
		for(const auto& buf : res.uniform_buffers)
		{
			const auto& type        = compiler.get_type(buf.base_type_id);
			const auto  size        = compiler.get_declared_struct_size(type);
			const auto  binding     = compiler.get_decoration(buf.id, spv::DecorationBinding);
			const auto  memberCount = type.member_types.size();

			puts(fmt::format("|-{}\n"
			                 "  |-binding: {}\n"
			                 "  |-size   : {} bytes\n"
			                 "  `-members",
			                 buf.name.empty() ? "NULL" : buf.name, binding, size, memberCount)
			         .c_str());

			for(size_t i = 0; i < memberCount; i++)
			{
				const auto&  memberName   = compiler.get_member_name(buf.base_type_id, i);
				const auto&  memberType   = compiler.get_type(type.member_types[i]);
				const size_t memberSize   = compiler.get_declared_struct_member_size(type, i);
				const size_t memberOffset = compiler.type_struct_member_offset(type, i);

				puts(fmt::format("    |-{}\n"
				                 "      |-offset: {}\n"
				                 "      |-type  : {}\n"
				                 "      `-size  : {} bytes",
				                 memberName.empty() ? "NULL" : memberName, memberOffset, "", memberSize)
				         .c_str());
			}
		}
	}
}

GLShader::~GLShader()
{
	glDeleteShader(handle);
}

size_t GLShader::getNativeHandle() const
{
	return handle;
}
