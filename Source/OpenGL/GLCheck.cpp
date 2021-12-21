// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLCheck.h"
#include <stdexcept>
#include <string>
#include <glad/glad.h>
// #include <format>

using namespace std::literals::string_literals;

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
		// std::format("OpenGL error ({}): {}", error, GLGetErrorString(error))
		throw std::runtime_error("OpenGL error ("s + std::to_string(error) + "): " + GLGetErrorString(error));
	}
}

void GLClearError()
{
	while(glGetError() != GL_NO_ERROR)
		;
}
