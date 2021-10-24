// Copyright 2021 SMS
// License(Apache-2.0)

#include "IndexBuffer.h"
#include "Renderer.h"

#include "OpenGL/GLIndexBuffer.h"

std::shared_ptr<IndexBuffer> IndexBuffer::create(const std::vector<value_type>& data, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		;// return std::make_shared<GLVertexBuffer>(data.data(), data.size() * sizeof(value_type), usage);
	}
	return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(const void* data, size_t size, Usage usage)
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		;// return std::make_shared<GLVertexBuffer>(data, size, usage);
	}
	return nullptr;
}