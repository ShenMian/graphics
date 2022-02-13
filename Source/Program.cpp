// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Program.h"
#include "Renderer.h"
#include <cassert>
#include <filesystem>
#include <vector>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

#include "OpenGL/GLProgram.h"
#include "Vulkan/VKProgram.h"

namespace fs = std::filesystem;

std::shared_ptr<Program> Program::create(const Descriptor& desc)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		return std::make_shared<GLProgram>(desc);

	case Vulkan:
		return std::make_shared<VKProgram>(desc);
	}
	return nullptr;
}

std::shared_ptr<Program> Program::create(const std::string& name)
{
	Descriptor desc;

	desc.name = name;

	const std::vector<std::pair<std::string, Shader::Stage>> com = {
		{"vert", Shader::Stage::Vertex},
		{"frag", Shader::Stage::Fragment},
		{"geom", Shader::Stage::Geometry},
		{"comp", Shader::Stage::Compute}
	};

	for(const auto& [stageName, stage] : com)
	{
		auto path = fmt::format("{}.{}.spv", name, stageName);
		if(fs::exists(path))
		{
			Shader::Descriptor shaderDesc;
			shaderDesc.stage = stage;
			shaderDesc.path = path;

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
	}

	return create(desc);
}

int Program::getStageCount() const
{
	return stageCount;
}

Program::Program(const Descriptor& desc)
	: name(desc.name)
{
}
