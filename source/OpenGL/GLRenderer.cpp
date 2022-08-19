// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLRenderer.h"
#include <stdexcept>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/std.h>

std::string GLRenderer::getDeviceName() const
{
	return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

std::string GLRenderer::getRendererName() const
{
	return std::string("OpenGL ") + reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

std::string GLRenderer::getVendorName() const
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

void GLRenderer::init(const Window& win)
{
	glfwMakeContextCurrent(win.getHandle());
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		throw std::runtime_error("Glad: failed to init");

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(
	    [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, GLchar const* message,
	       void const* user_param) {
		    auto sourceToString = [](GLenum source) {
			    switch(source)
			    {
			    case GL_DEBUG_SOURCE_API:
				    return "API";
			    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
				    return "WINDOW SYSTEM";
			    case GL_DEBUG_SOURCE_SHADER_COMPILER:
				    return "SHADER COMPILER";
			    case GL_DEBUG_SOURCE_THIRD_PARTY:
				    return "THIRD PARTY";
			    case GL_DEBUG_SOURCE_APPLICATION:
				    return "APPLICATION";
			    case GL_DEBUG_SOURCE_OTHER:
				    return "OTHER";
			    }
		    };

		    auto typeToString = [](GLenum type) {
			    switch(type)
			    {
			    case GL_DEBUG_TYPE_ERROR:
				    return "ERROR";
			    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				    return "DEPRECATED_BEHAVIOR";
			    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				    return "UNDEFINED_BEHAVIOR";
			    case GL_DEBUG_TYPE_PORTABILITY:
				    return "PORTABILITY";
			    case GL_DEBUG_TYPE_PERFORMANCE:
				    return "PERFORMANCE";
			    case GL_DEBUG_TYPE_MARKER:
				    return "MARKER";
			    case GL_DEBUG_TYPE_OTHER:
				    return "OTHER";
			    }
		    };

		    auto severityToString = [](GLenum severity) {
			    switch(severity)
			    {
			    case GL_DEBUG_SEVERITY_NOTIFICATION:
				    return "NOTIFICATION";
			    case GL_DEBUG_SEVERITY_LOW:
				    return "LOW";
			    case GL_DEBUG_SEVERITY_MEDIUM:
				    return "MEDIUM";
			    case GL_DEBUG_SEVERITY_HIGH:
				    return "HIGH";
			    }
		    };

			if(severity == GL_DEBUG_SEVERITY_NOTIFICATION)
			    return;

		    puts(fmt::format("OpenGL Message\n"
		                     "|-Source:   {}\n"
		                     "|-Type:     {}\n"
		                     "|-Severity: {}\n"
		                     "`-Message:  {}",
		                     sourceToString(source), typeToString(type), severityToString(severity), message)
		             .c_str());
	    },
	    nullptr);
}

void GLRenderer::deinit()
{
	glfwMakeContextCurrent(nullptr);
}
