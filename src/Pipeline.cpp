// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Pipeline.h"

#include "OpenGL/GLPipeline.h"
#include "Vulkan/VKPipeline.h"

std::shared_ptr<Pipeline> Pipeline::create(const Descriptor& desc)
{
	switch(Renderer::get_backend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		return std::make_shared<GLPipeline>(desc);

	case Vulkan:
		return std::make_shared<VKPipeline>(desc);
	}
	return nullptr;
}

Pipeline::Pipeline(const Descriptor& desc) : desc(desc)
{
}
