// Copyright 2022 ShenMian
// License(Apache-2.0)


#include "GLProgram.h"
#include "GLShader.h"
#include <cassert>
#include <filesystem>
#include <glad/glad.h>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

GLProgram::GLProgram(const Descriptor& desc) : Program(desc), handle_(glCreateProgram())
{
	if(desc.vertex == nullptr || desc.fragment == nullptr)
		throw std::runtime_error("program must have vertex shader and fragment shader");

	attach(desc.vertex);
	attach(desc.fragment);
	attach(desc.geometry);
	attach(desc.compute);
	link();
	detach(desc.vertex);
	detach(desc.fragment);
	detach(desc.geometry);
	detach(desc.compute);
}

GLProgram::~GLProgram()
{
	glDeleteProgram(handle_);
}

void GLProgram::use()
{
	glUseProgram(handle_);
}

void GLProgram::set_uniform(const std::string& name, int value)
{
	use();
	glUniform1i(get_uniform_location(name), value);
}

void GLProgram::set_uniform(const std::string& name, float value)
{
	use();
	glUniform1f(get_uniform_location(name), value);
}

void GLProgram::set_uniform(const std::string& name, const Vector2& value)
{
	use();
	glUniform2f(get_uniform_location(name), value.x(), value.y());
}

void GLProgram::set_uniform(const std::string& name, const Vector3& value)
{
	use();
	glUniform3f(get_uniform_location(name), value.x(), value.y(), value.z());
}

void GLProgram::set_uniform(const std::string& name, const Vector4& value)
{
	use();
	glUniform4f(get_uniform_location(name), value.x(), value.y(), value.z(), value.w());
}

void GLProgram::set_uniform(const std::string& name, const Matrix4& value)
{
	use();
	glUniformMatrix4fv(get_uniform_location(name), 1, false, value.data());
}

GLProgram::operator GLuint() const noexcept
{
	return handle_;
}

int GLProgram::get_uniform_buffer_location(const std::string& name) const
{
	return glGetUniformBlockIndex(handle_, name.c_str());
}

int GLProgram::get_uniform_location(const std::string& name) const
{
	// FIXME: SPRI-V shader 无法再正确识别该项
	const auto it = uniform_location_cache_.find(name);
	if(it == uniform_location_cache_.end())
	{
		const auto location = glGetUniformLocation(handle_, name.c_str());
		if(location == -1)
			throw std::runtime_error("uniform buffer name do not exist");
		uniform_location_cache_[name] = location;
		return location;
	}
	return it->second;
}

void GLProgram::attach(const std::shared_ptr<Shader> shader)
{
	if(shader == nullptr)
		return;
	stage_count_++;
	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glAttachShader(handle_, static_cast<GLuint>(glShader->get_handle()));
}

void GLProgram::detach(const std::shared_ptr<Shader> shader)
{
	if(shader == nullptr)
		return;
	stage_count_++;
	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glDetachShader(handle_, static_cast<GLuint>(glShader->get_handle()));
}

void GLProgram::link()
{
	glLinkProgram(handle_);

	// 获取链接结果状态
	int status;
	glGetProgramiv(handle_, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		// 获取报错内容
		int size;
		glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &size);
		std::string info(size, '\0');
		glGetProgramInfoLog(handle_, (GLsizei)info.size(), &size, info.data());
		throw std::runtime_error(fmt::format("program '{}' link failed: {}", name_, info));
	}
}
