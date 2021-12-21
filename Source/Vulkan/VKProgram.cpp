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
}

void VKProgram::link()
{
}
