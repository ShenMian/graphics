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

std::string ToString(spirv_cross::SPIRType type)
{
	std::string result;

	switch(type.basetype)
	{
		using enum spirv_cross::SPIRType::BaseType;

	case Void:
		result += "Void";
		break;

	case Boolean:
		result += "Boolean";
		break;

	case SByte:
		result += "SByte";
		break;

	case UByte:
		result += "UByte";
		break;

	case Short:
		result += "Short";
		break;

	case UShort:
		result += "UShort";
		break;

	case Int:
		result += "Int";
		break;

	case UInt:
		result += "UInt";
		break;

	case Int64:
		result += "Int64";
		break;

	case UInt64:
		result += "UInt64";
		break;

	case AtomicCounter:
		result += "AtomicCounter";
		break;

	case Half:
		result += "Half";
		break;

	case Float:
		result += "Float";
		break;

	case Double:
		result += "Double";
		break;

	case Struct:
		result += "Struct";
		break;

	case Image:
		result += "Image";
		break;

	case SampledImage:
		result += "SampledImage";
		break;

	case Sampler:
		result += "Sampler";
		break;

	default:
		result += "Unknown";
		break;
	}

	if(type.columns > 1)
	{
		result += " (matrix)";
	}

	return result;
}

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

		size_t level = 0;

		fmt::print("{:{}}{}\n", "", level, "Sampled images");
		for(size_t i = 0; i < res.sampled_images.size(); i++)
		{
			const auto& img      = res.sampled_images[i];
			const auto  binding  = compiler.get_decoration(img.id, spv::DecorationBinding);
			const auto  location = compiler.get_decoration(img.id, spv::DecorationLocation);

			fmt::print("{:{}}- {}\n", "", level, img.name);

			level += 2;
			fmt::print("{:{}}{:8}: {}\n", "", level, "binding", binding);
			fmt::print("{:{}}{:8}: {}\n", "", level, "location", location);
			level -= 2;
		}

		fmt::print("{:{}}{}\n", "", level, "Uniform buffers");
		for(size_t i = 0; i < res.uniform_buffers.size(); i++)
		{
			const auto& buf = res.uniform_buffers[i];

			const auto& type        = compiler.get_type(buf.base_type_id);
			const auto  size        = compiler.get_declared_struct_size(type);
			const auto  binding     = compiler.get_decoration(buf.id, spv::DecorationBinding);
			const auto  memberCount = type.member_types.size();

			fmt::print("{:{}}- {}\n", "", level, buf.name.empty() ? "NULL" : buf.name);

			level += 2;
			fmt::print("{:{}}{:7}: {}\n", "", level, "binding", binding);
			fmt::print("{:{}}{:7}: {}\n", "", level, "size", size);
			fmt::print("{:{}}{}\n", "", level, "members");

			for(size_t j = 0; j < memberCount; j++)
			{
				const auto&  memberName   = compiler.get_member_name(buf.base_type_id, j);
				const auto&  memberType   = compiler.get_type(type.member_types[j]);
				const size_t memberSize   = compiler.get_declared_struct_member_size(type, j);
				const size_t memberOffset = compiler.type_struct_member_offset(type, j);

				fmt::print("{:{}}- {}\n", "", level, memberName.empty() ? "NULL" : memberName);

				level += 2;
				fmt::print("{:{}}{:6}: {}\n", "", level, "offset", memberOffset);
				fmt::print("{:{}}{:6}: {}\n", "", level, "size", memberSize);
				fmt::print("{:{}}{:6}: {}\n", "", level, "type", ToString(memberType));

				if(!memberType.array.empty())
				{
					const auto arrayStride = compiler.type_struct_member_array_stride(type, j);
					fmt::print("{:{}}{:12}: {}\n", "", level, "array size", memberType.array.size());
					fmt::print("{:{}}{:12}: {}\n", "", level, "array stride", arrayStride);
				}
				level -= 2;
			}
			level -= 2;
		}
	}
}

GLShader::~GLShader()
{
	glDeleteShader(handle);
}

size_t GLShader::getHandle() const
{
	return handle;
}
