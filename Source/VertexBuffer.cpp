// Copyright 2021 SMS
// License(Apache-2.0)

#include "VertexBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLVertexBuffer.h"

std::shared_ptr<VertexBuffer> VertexBuffer::create(const std::vector<value_type>& data, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		;// return std::make_shared<GLVertexBuffer>(data.data(), data.size() * sizeof(value_type), usage);
	}
	return nullptr;
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* data, size_t size, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		;// return std::make_shared<GLVertexBuffer>(data, size, usage);
	}
	return nullptr;
}