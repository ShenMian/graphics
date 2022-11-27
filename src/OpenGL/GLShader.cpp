// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLShader.h"
#include <cassert>
#include <fstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
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

} // namespace

GLShader::GLShader(const Descriptor& desc) : Shader(desc), handle(glCreateShader(GLStage[stage]))
{
	auto buf = getCode(desc.path);
	parse(buf);

	glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, buf.data(),
	               static_cast<GLsizei>(buf.size() * sizeof(uint32_t)));
	glSpecializeShader(handle, entryPoint.c_str(), 0, nullptr, nullptr);
}

GLShader::~GLShader()
{
	glDeleteShader(handle);
}

size_t GLShader::getHandle() const
{
	return handle;
}
