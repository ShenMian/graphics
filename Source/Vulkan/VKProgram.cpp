// Copyright 2021 ShenMian
// License(Apache-2.0)


#include "VKProgram.h"
#include "VKShader.h"
#include <cassert>
#include <stdexcept>
#include <filesystem>

namespace fs = std::filesystem;

namespace
{

std::unordered_map<Shader::Stage, VkShaderStageFlagBits> VKStage = {
	{Shader::Stage::Vertex, VK_SHADER_STAGE_VERTEX_BIT},
	{Shader::Stage::Fragment, VK_SHADER_STAGE_FRAGMENT_BIT},
	{Shader::Stage::Geometry, VK_SHADER_STAGE_GEOMETRY_BIT},
	{Shader::Stage::Compute, VK_SHADER_STAGE_COMPUTE_BIT}
};

}

VKProgram::VKProgram(const std::string& name)
	: Program(name)
{
	if(fs::exists(name + ".vert.spv") || fs::exists(name + ".vert.glsl"))
		addShader(Shader::create(name, Shader::Stage::Vertex));
	if(fs::exists(name + ".geom.spv") || fs::exists(name + ".geom.glsl"))
		addShader(Shader::create(name, Shader::Stage::Geometry));
	if(fs::exists(name + ".frag.spv") || fs::exists(name + ".frag.glsl"))
		addShader(Shader::create(name, Shader::Stage::Fragment));
	if(fs::exists(name + ".comp.spv") || fs::exists(name + ".comp.glsl"))
		addShader(Shader::create(name, Shader::Stage::Compute));
	if(!shaders.contains(Shader::Stage::Vertex) || !shaders.contains(Shader::Stage::Fragment))
		throw std::runtime_error("program must have vertex shader and fragment shader");

	for(auto& [stage, shader] : shaders)
	{
		VkPipelineShaderStageCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.stage = VKStage[stage];
		info.module = shader->getNativeHandle();
		info.pName = "main";
		infos.push_back(info);
	}
}

VKProgram::~VKProgram()
{
}

std::vector<VkPipelineShaderStageCreateInfo>& VKProgram::getInfos()
{
	return infos;
}

void VKProgram::addShader(std::shared_ptr<Shader> shader)
{
	shaders.emplace(shader->getStage(), std::dynamic_pointer_cast<VKShader>(shader));
	stageCount++;
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
