// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "GLProgram.h"
#include "GLShader.h"
#include "GLCheck.h"
#include "../Program.h"
#include <cassert>
#include <format>
#include <filesystem>
#include <glad/glad.h>

namespace fs = std::filesystem;

GLProgram::GLProgram(const std::string& name)
	: Program(name)
{
	handle = glCreateProgram();

	std::shared_ptr<Shader> vert, geom, frag, comp;

	if(fs::exists(name + ".vert.spv") || fs::exists(name + ".vert.glsl"))
	{
		vert = Shader::create(name, Shader::Stage::Vertex);
		attach(vert);
	}
	if(fs::exists(name + ".geom.spv") || fs::exists(name + ".geom.glsl"))
	{
		geom = Shader::create(name, Shader::Stage::Geometry);
		attach(geom);
	}
	if(fs::exists(name + ".frag.spv") || fs::exists(name + ".frag.glsl"))
	{
		frag = Shader::create(name, Shader::Stage::Fragment);
		attach(frag);
	}
	if(fs::exists(name + ".comp.spv") || fs::exists(name + ".comp.glsl"))
	{
		comp = Shader::create(name, Shader::Stage::Compute);
		attach(comp);
	}
	assert(vert && frag && "program must have vertex shader and fragment shader");

	link();
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

/*
void GLProgram::uploadUniform(const std::string& name, const Vector2& value)
{
	bind();
	glUniform2f(getUniformLocation(name), value.x, value.y);
	GLCheckError();
}

void GLProgram::uploadUniform(const std::string& name, const Vector3& value)
{
	bind();
	glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	GLCheckError();
}

void GLProgram::uploadUniform(const std::string& name, const Vector4& value)
{
	bind();
	glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	GLCheckError();
}

void GLProgram::uploadUniform(const std::string& name, const Matrix4& value)
{
	bind();
	glUniformMatrix4fv(getUniformLocation(name), 1, false, value.data());
	GLCheckError();
}
*/

int GLProgram::getUniformLocation(const std::string& name)
{
	const auto it = uniformLocations.find(name);
	if(it == uniformLocations.end())
	{
		const auto location = glGetUniformLocation(handle, name.c_str());
		if(location != -1)
			throw std::exception(std::format("uniform '{}' doesn't exist", name).c_str());
		uniformLocations[name] = location;
		return location;
	}
	return it->second;
}

void GLProgram::attach(const std::shared_ptr<Shader> shader)
{
	glAttachShader(handle, (GLuint)shader->getNativeHandle());
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
		// assert(false && std::format("program '{}' link failure: {}", name, info));
	}
}
