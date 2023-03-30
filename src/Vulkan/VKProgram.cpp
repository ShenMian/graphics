// Copyright 2022 ShenMian
// License(Apache-2.0)


#include "VKProgram.h"
#include "VKShader.h"
#include <cassert>
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

namespace
{

std::unordered_map<Shader::Stage, VkShaderStageFlagBits> VKStage = {
    {Shader::Stage::Vertex, VK_SHADER_STAGE_VERTEX_BIT},
    {Shader::Stage::Fragment, VK_SHADER_STAGE_FRAGMENT_BIT},
    {Shader::Stage::Geometry, VK_SHADER_STAGE_GEOMETRY_BIT},
    {Shader::Stage::Compute, VK_SHADER_STAGE_COMPUTE_BIT}};

}

VKProgram::VKProgram(const Descriptor& desc) : Program(desc)
{
	if(desc.vertex == nullptr || desc.fragment == nullptr)
		throw std::runtime_error("program must have vertex shader and fragment shader");

	add_shader(desc.vertex);
	add_shader(desc.fragment);
	add_shader(desc.geometry);
	add_shader(desc.compute);

	for(auto& [stage, shader] : shaders_)
	{
		VkPipelineShaderStageCreateInfo info = {};
		info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		info.stage                           = VKStage[stage];
		info.module                          = shader->get_handle();
		info.pName                           = shader->get_entry_point().c_str();
		infos_.push_back(info);
	}
}

VKProgram::~VKProgram()
{
}

std::vector<VkPipelineShaderStageCreateInfo>& VKProgram::getInfos()
{
	return infos_;
}

void VKProgram::add_shader(std::shared_ptr<Shader> shader)
{
	if(shader == nullptr)
		return;
	shaders_.emplace(shader->get_stage(), std::dynamic_pointer_cast<VKShader>(shader));
	stage_count_++;
}

void VKProgram::use()
{
}

void VKProgram::set_uniform(const std::string& name, int value)
{
}

void VKProgram::set_uniform(const std::string& name, float value)
{
}

void VKProgram::set_uniform(const std::string& name, const Vector2& value)
{
}

void VKProgram::set_uniform(const std::string& name, const Vector3& value)
{
}

void VKProgram::set_uniform(const std::string& name, const Vector4& value)
{
}

void VKProgram::set_uniform(const std::string& name, const Matrix4& value)
{
}
