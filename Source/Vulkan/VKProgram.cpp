// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "VKProgram.h"
#include "VKShader.h"
#include <cassert>
#include <stdexcept>
#include <filesystem>
// #include <format>

namespace fs = std::filesystem;

VKProgram::VKProgram(const std::string& name)
	: Program(name)
{
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
	if(vert == nullptr || frag == nullptr)
		throw std::runtime_error("program must have vertex shader and fragment shader");

	link();
}

VKProgram::~VKProgram()
{
}

void VKProgram::use()
{
}

void VKProgram::setUniform(const std::string& name, int value)
{
}

void VKProgram::setUniform(const std::string& name, float value)
{
}

void VKProgram::setUniform(const std::string& name, const Vector2& value)
{
}

void VKProgram::setUniform(const std::string& name, const Vector3& value)
{
}

void VKProgram::setUniform(const std::string& name, const Vector4& value)
{
}

void VKProgram::setUniform(const std::string& name, const Matrix4& value)
{
}

int VKProgram::getUniformLocation(const std::string& name)
{
	return 0;
}

void VKProgram::attach(const std::shared_ptr<Shader> shader)
{
	stageCount++;
}

void VKProgram::link()
{
}
