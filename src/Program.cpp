// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Program.h"
#include "Renderer.h"
#include <cassert>
#include <filesystem>
#include <vector>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

#include "OpenGL/GLProgram.h"
#include "Vulkan/VKProgram.h"

namespace fs = std::filesystem;

std::shared_ptr<Program> Program::create(const Descriptor& desc)
{
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		return std::make_shared<GLProgram>(desc);

	case Vulkan:
		return std::make_shared<VKProgram>(desc);
	}
	return nullptr;
}

std::shared_ptr<Program> Program::create(std::string_view name)
{
	Descriptor desc;

	desc.name = name;

	const std::vector<std::pair<std::string, Shader::Stage>> com = {{"vert", Shader::Stage::Vertex},
	                                                                {"frag", Shader::Stage::Fragment},
	                                                                {"geom", Shader::Stage::Geometry},
	                                                                {"comp", Shader::Stage::Compute}};

	// GLSL
	for(const auto& [stageName, stage] : com)
	{
		const auto path = fmt::format("{}.{}.glsl", name, stageName);
		if(!fs::exists(path))
			continue;

		Shader::Descriptor shaderDesc;
		shaderDesc.stage = stage;
		shaderDesc.path  = path;

		switch(stage)
		{
			using enum Shader::Stage;

		case Vertex:
			desc.vertex = Shader::create(shaderDesc);
			break;

		case Fragment:
			desc.fragment = Shader::create(shaderDesc);
			break;

		case Geometry:
			desc.geometry = Shader::create(shaderDesc);
			break;

		case Compute:
			desc.compute = Shader::create(shaderDesc);
			break;
		}
	}
	if(desc.vertex != nullptr && desc.fragment != nullptr)
		return create(desc);

	// SPV
	for(const auto& [stageName, stage] : com)
	{
		auto path = fmt::format("{}.{}.spv", name, stageName);
		if(!fs::exists(path))
			continue;

		Shader::Descriptor shaderDesc;
		shaderDesc.stage = stage;
		shaderDesc.path  = path;

		switch(stage)
		{
			using enum Shader::Stage;

		case Vertex:
			desc.vertex = Shader::create(shaderDesc);
			break;

		case Fragment:
			desc.fragment = Shader::create(shaderDesc);
			break;

		case Geometry:
			desc.geometry = Shader::create(shaderDesc);
			break;

		case Compute:
			desc.compute = Shader::create(shaderDesc);
			break;
		}
	}

	return create(desc);
}

int Program::getStageCount() const
{
	return stageCount;
}

Program::Program(const Descriptor& desc) : name(desc.name)
{
}
