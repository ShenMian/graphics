// Copyright 2021 SMS
// License(Apache-2.0)

#include "Renderer.h"
#include <cassert>
#include <format>

Renderer::API Renderer::api = Renderer::API::OpenGL;

void Renderer::setAPI(API newAPI)
{
	api = newAPI;
}

Renderer::API Renderer::getAPI()
{
	return api;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* GLGetErrorString(GLenum error)
{
	switch(error)
	{
	case GL_NO_ERROR:
		return "GL_NO_ERROR";

	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";

	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";

	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";

	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";

	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";

	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "GL_INVALID_FRAMEBUFFER_OPERATION";

	default:
		return "unknown";
	}
}

void GLCheckError()
{
	while(const auto error = glGetError())
	{
		if(error == GL_NO_ERROR)
			break;
		throw std::exception(std::format("OpenGL error ({}): {}", error, GLGetErrorString(error)).c_str());
	}
}

void GLClearError()
{
	while(glGetError() != GL_NO_ERROR)
		;
}