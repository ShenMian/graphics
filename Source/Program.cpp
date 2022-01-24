// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Program.h"
#include "Renderer.h"
#include <cassert>

#include "OpenGL/GLProgram.h"
#include "Vulkan/VKProgram.h"

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

Program::Program(const std::string& name)
	: name(name)
{
}

int Program::getStageCount() const
{
	return stageCount;
}

Program::Program(const Descriptor& desc)
	: name(desc.name)
{
}
