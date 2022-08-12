// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLCheck.h"
#include <glad/glad.h>
#include <stdexcept>
#include <string>

#define FMT_HEADER_ONLY
#include <fmt/core.h>

using namespace std::literals::string_literals;

namespace
{

std::string_view GLGetErrorString(GLenum error)
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

} // namespace

void GLCheckError()
{
	while(const auto error = glGetError())
		throw std::runtime_error(fmt::format("OpenGL error ({}): {}", error, GLGetErrorString(error)));
}

void GLClearError()
{
	while(glGetError() != GL_NO_ERROR)
		;
}
