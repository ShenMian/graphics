// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Pipeline.h"

#include "Vulkan/VKPipeline.h"

std::shared_ptr<Pipeline> Pipeline::create(const Descriptor& desc)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case Vulkan:
		return std::make_shared<VKPipeline>(desc);
	}
	return nullptr;
}

Pipeline::Pipeline(const Descriptor& desc)
	: program(desc.program)
{
}
