// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Shader.h"
#include "Renderer.h"
#include <cassert>
#include <stdexcept>
#include <fstream>
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>

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

std::string ToString(spirv_cross::SPIRType type)
{
	std::string result;

	switch(type.basetype)
	{
		using enum spirv_cross::SPIRType::BaseType;

	case Void:
		result += "void";
		break;

	case Boolean:
		result += "boolean";
		break;

	case SByte:
		result += "sbyte";
		break;

	case UByte:
		result += "ubyte";
		break;

	case Short:
		result += "short";
		break;

	case UShort:
		result += "ushort";
		break;

	case Int:
		result += "int";
		break;

	case UInt:
		result += "uint";
		break;

	case Int64:
		result += "int64";
		break;

	case UInt64:
		result += "uint64";
		break;

	case AtomicCounter:
		result += "atomicCounter";
		break;

	case Half:
		result += "half";
		break;

	case Float:
		result += "float";
		break;

	case Double:
		result += "double";
		break;

	case Struct:
		result += "struct";
		break;

	case Image:
		result += "image";
		break;

	case SampledImage:
		result += "sampled image";
		break;

	case Sampler:
		result += "sampler";
		break;

	default:
		result += "unknown";
		break;
	}

	if(type.columns > 1)
	{
		result += " (matrix)";
	}

	return result;
}

} // namespace

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

const std::string& Shader::getName() const noexcept
{
	return name;
}

const std::string& Shader::getEntryPoint() const noexcept
{
	return entryPoint;
}

Shader::Stage Shader::getStage() const noexcept
{
	return stage;
}

Shader::Shader(const Descriptor& desc) : name(desc.path.filename().string()), stage(desc.stage)
{
}

std::vector<uint32_t> Shader::getCode(fs::path path)
{
	if(!fs::exists(path))
		throw std::runtime_error(fmt::format("no such file: {}", path));

	if(path.extension() != ".spv")
	{
		compile(path, fs::path(path).replace_extension(".spv"), stage);
		path.replace_extension(".spv");
	}

	const auto    fileSize = fs::file_size(path);
	std::ifstream file(path, std::ios::binary);
	if(!file)
		throw std::runtime_error(fmt::format("failed to open file: {}", path));

	std::vector<uint32_t> buf(fileSize / sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(buf.data()), fileSize);
	if(!file || file.gcount() != fileSize)
		throw std::runtime_error(fmt::format("failed to read file: {}", path));
	file.close();

	return buf;
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
	if(!file)
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

void Shader::parse(const std::vector<uint32_t>& buf)
{
	spirv_cross::Compiler compiler(buf);
	assert(compiler.get_entry_points_and_stages().size() == 1);
	entryPoint = compiler.get_entry_points_and_stages().front().name;

	// 反射并输出结果
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
