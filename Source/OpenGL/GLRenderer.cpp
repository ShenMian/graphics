// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "GLRenderer.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

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
		throw std::runtime_error("OpenGL(Glad) init failed");
}

void GLRenderer::deinit()
{
	glfwMakeContextCurrent(nullptr);
}
