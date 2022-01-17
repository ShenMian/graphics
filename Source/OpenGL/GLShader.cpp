﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLShader.h"
#include <fstream>
#include <glad/glad.h>
#include <unordered_map>
#include <cassert>
#include <stdexcept>
#include <vector>
// #include <format>

namespace fs = std::filesystem;

namespace
{

std::unordered_map<Shader::Stage, GLenum> GLStage = {
	{Shader::Stage::Vertex, GL_VERTEX_SHADER},
	{Shader::Stage::Fragment, GL_FRAGMENT_SHADER},
	{Shader::Stage::Geometry, GL_GEOMETRY_SHADER},
	{Shader::Stage::Compute, GL_COMPUTE_SHADER}
};

}

GLShader::GLShader(const Descriptor& desc)
	: Shader(desc)
{
	handle = glCreateShader(GLStage[stage]);

	if(!fs::exists(desc.path))
		throw std::runtime_error("file not found: " + desc.path.string());

	// 读取文件内容
	const auto fileSize = fs::file_size(desc.path);
	std::ifstream file(desc.path, std::ios::binary);
	if(!file.is_open())
		throw std::runtime_error("failed to open file: " + desc.path.string());

	std::vector<char> buffer(fileSize);
	file.read(buffer.data(), fileSize);
	if(!file.good() || file.gcount() != fileSize)
		throw std::runtime_error("failed to read file: " + desc.path.string());
	file.close();

	if(desc.path.extension() == ".spv")
	{
		glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(), (GLsizei)buffer.size());
		glSpecializeShader(handle, "main", 0, nullptr, nullptr); // 指定入口点函数名称
		return;
	}

	// 编译源代码
	buffer.push_back('\0');
	const auto strings = buffer.data();
	glShaderSource(handle, 1, &strings, nullptr);
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
		throw std::runtime_error("shader compile error: " + desc.path.filename().string() + ":" + info);
	}
}

GLShader::GLShader(const fs::path& path, Shader::Stage stage)
	: Shader(path.string(), stage)
{
	handle = glCreateShader(GLStage[stage]);

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

	if(path.extension() == ".spv")
	{
		glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(), (GLsizei)buffer.size());
		glSpecializeShader(handle, "main", 0, nullptr, nullptr); // 指定入口点函数名称
		return;
	}

	// 编译源代码
	buffer.push_back('\0');
	const auto strings = buffer.data();
	glShaderSource(handle, 1, &strings, nullptr);
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
		throw std::runtime_error("shader compile error: " + path.filename().string() + ":" + info);
	}
}

GLShader::GLShader(const std::string& name, Stage stage)
	: Shader(name, stage)
{
	handle = glCreateShader(GLStage[stage]);
	load();
}

GLShader::~GLShader()
{
	glDeleteShader(handle);
}

void GLShader::load()
{
	// 从 SPIR-V 文件载入
	// std::format("{}.{}.spv", name, extension[stage])
	if(fs::exists(name + "." + extension[stage] + ".spv"))
	{
		const fs::path path = name + "." + extension[stage] + ".spv";
		const auto     size = fs::file_size(path);

		std::ifstream file(path, std::ios::binary);
		assert(file.is_open());

		std::vector<std::byte> buffer(size);
		file.read(reinterpret_cast<char*>(buffer.data()), size);
		assert(file.good() && file.gcount() == size);
		file.close();

		glShaderBinary(1, &handle, GL_SHADER_BINARY_FORMAT_SPIR_V, buffer.data(), (GLsizei)buffer.size());
		glSpecializeShader(handle, "main", 0, nullptr, nullptr); // 指定入口点函数名称

		return;
	}

	// 从源文件载入
	// std::format("{}.{}.glsl", name, extension[stage])
	else if(fs::exists(name + "." + extension[stage] + ".glsl"))
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

		// 编译源代码
		const auto strings = buffer.data();
		glShaderSource(handle, 1, &strings, nullptr);
		glCompileShader(handle);

		// 获取编译结果状态
		int status;
		glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
		if(status == GL_FALSE)
		{
			// 获取报错内容
			int size;
			glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &size);
			std::string info(size, '\0');
			glGetShaderInfoLog(handle, (GLsizei)info.size(), &size, info.data());
			throw std::runtime_error("shader '" + path.string() + "' compile error: " + info);
		}

		return;
	}

	assert(false);
}

size_t GLShader::getNativeHandle() const
{
	return handle;
}
