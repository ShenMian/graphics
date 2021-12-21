// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLRenderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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
	auto handle = reinterpret_cast<GLFWwindow*>(win.getNativeHandle());
	glfwMakeContextCurrent(handle);
	static auto ret = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	if(!ret)
		throw std::runtime_error("glad init failed");
}
