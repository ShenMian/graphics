// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "GLProgram.h"
#include "GLCheck.h"
#include "GLShader.h"
#include <cassert>
#include <filesystem>
#include <glad/glad.h>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace fs = std::filesystem;

GLProgram::GLProgram(const Descriptor& desc) : Program(desc), handle(glCreateProgram())
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
	glDeleteProgram(handle);
}

void GLProgram::use()
{
	glUseProgram(handle);
	GLCheckError();
}

void GLProgram::setUniform(const std::string& name, int value)
{
	use();
	glUniform1i(getUniformLocation(name), value);
	GLCheckError();
}

void GLProgram::setUniform(const std::string& name, float value)
{
	use();
	glUniform1f(getUniformLocation(name), value);
	GLCheckError();
}

void GLProgram::setUniform(const std::string& name, const Vector2& value)
{
	use();
	glUniform2f(getUniformLocation(name), value.x, value.y);
	GLCheckError();
}

void GLProgram::setUniform(const std::string& name, const Vector3& value)
{
	use();
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	GLCheckError();
}

void GLProgram::setUniform(const std::string& name, const Vector4& value)
{
	use();
	glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	GLCheckError();
}

void GLProgram::setUniform(const std::string& name, const Matrix4& value)
{
	use();
	glUniformMatrix4fv(getUniformLocation(name), 1, false, value.data());
	GLCheckError();
}

GLProgram::operator GLuint() const
{
	return handle;
}

int GLProgram::getUniformBufferLocation(const std::string& name)
{
	return glGetUniformBlockIndex(handle, name.c_str());
}

int GLProgram::getUniformLocation(const std::string& name)
{
	// FIXME: SPRI-V Shader 无法再正确识别该项
	const auto it = uniformLocations.find(name);
	if(it == uniformLocations.end())
	{
		const auto location = glGetUniformLocation(handle, name.c_str());
		if(location == -1)
			throw std::runtime_error("uniform buffer name do not exist");
		uniformLocations[name] = location;
		return location;
	}
	return it->second;
}

void GLProgram::attach(const std::shared_ptr<Shader> shader)
{
	if(shader == nullptr)
		return;
	stageCount++;
	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glAttachShader(handle, (GLuint)glShader->getNativeHandle());
}

void GLProgram::detach(const std::shared_ptr<Shader> shader)
{
	if(shader == nullptr)
		return;
	stageCount++;
	auto glShader = std::dynamic_pointer_cast<GLShader>(shader);
	glDetachShader(handle, (GLuint)glShader->getNativeHandle());
}

void GLProgram::link()
{
	glLinkProgram(handle);

	// 获取链接结果状态
	int status;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	if(status == GL_FALSE)
	{
		// 获取报错内容
		int size;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &size);
		std::string info(size, '\0');
		glGetProgramInfoLog(handle, (GLsizei)info.size(), &size, info.data());
		throw std::runtime_error(fmt::format("program '{}' link failed: {}", name, info));
	}
}
