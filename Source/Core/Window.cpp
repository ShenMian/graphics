// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Window.h"
#include "Image.h"
#include "Monitor.h"
#include "Renderer.h"
#include <cassert>
#include <stdexcept>
#include <GLFW/glfw3.h>
// #include <format>

using namespace std::literals::string_literals;
namespace fs = std::filesystem;

Window::Window(const std::string_view title, const Vector2i& size, bool fullscreen)
{
	handle = glfwCreateWindow(size.x, size.y, title.data(),
		fullscreen ? reinterpret_cast<GLFWmonitor*>(Monitor::getPrimary().getNativeHandle()) : nullptr, nullptr);
	assert(handle);

	glfwSetWindowUserPointer(handle, static_cast<void*>(this));

	// 开启 MASS 抗锯齿
	// glfwWindowHint(GLFW_SAMPLES, 2);
	// glEnable(GL_MULTISAMPLE);

	setupCallbacks();
}

Window::~Window()
{
	glfwDestroyWindow(handle);
}

void Window::update()
{
	if(Renderer::getAPI() == Renderer::API::OpenGL)
		glfwSwapBuffers(handle);
	glfwPollEvents();
}

void Window::setTitle(const std::string_view title)
{
	glfwSetWindowTitle(handle, title.data());
}

void Window::setSize(const Vector2i& size)
{
	glfwSetWindowSize(handle, size.x, size.y);
}

Vector2i Window::getSize() const noexcept
{
	int x, y;
	glfwGetWindowSize(handle, &x, &y);
	return {x, y};
}

void Window::setPosition(const Vector2i& pos)
{
	glfwSetWindowPos(handle, pos.x, pos.y);
}

Vector2i Window::getPosition() const noexcept
{
	Vector2i size;
	glfwGetWindowPos(handle, &size.x, &size.y);
	return size;
}

void Window::setVisible(bool visible) noexcept
{
	if(visible)
		glfwShowWindow(handle);
	else
		glfwHideWindow(handle);
}

bool Window::isVisible() const noexcept
{
	return glfwGetWindowAttrib(handle, GLFW_VISIBLE);
}

void Window::setFullscreen(bool fullscreen)
{
	if(fullscreen)
	{
		size = getSize();
		position = getPosition();
	}

	const auto& monitor = Monitor::getPrimary();
	glfwSetWindowMonitor(handle, fullscreen ? static_cast<GLFWmonitor*>(monitor.getNativeHandle()) : nullptr, position.x, position.y, size.x, size.y, GLFW_DONT_CARE);
}

bool Window::isFullscreen() const noexcept
{
	return glfwGetWindowMonitor(handle) != nullptr;
}

void Window::setSync(bool enable) noexcept
{
	if(enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void Window::setIcon(const Image& image)
{
	if(image.getSize().x > 48 && image.getSize().y > 48)
		throw std::runtime_error("image size too large");

	GLFWimage glfwImage;
	glfwImage.pixels = const_cast<unsigned char*>(image.getData());
	glfwImage.width = image.getSize().x;
	glfwImage.height = image.getSize().y;
	glfwSetWindowIcon(handle, 1, &glfwImage);
}

void Window::requestFocus()
{
	glfwRequestWindowAttention(handle);
}

void Window::setCursorLock(bool enable)
{
	glfwSetInputMode(handle, GLFW_CURSOR, enable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::setRawMouseMotion(bool enable)
{
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(handle, GLFW_RAW_MOUSE_MOTION, enable);
}

void* Window::getNativeHandle() const
{
	return handle;
}

void Window::setupCallbacks()
{
	glfwSetErrorCallback([](int error, const char* desc)
	{
		if(error == 65537)
			throw std::runtime_error("GLFW error: some windows has not been destroyed");
		throw std::runtime_error("GLFW error: "s + desc);
	});

	glfwSetWindowSizeCallback(handle, [](GLFWwindow* native, int width, int height)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onResize)
			handle->onResize({width, height});
	});

	glfwSetWindowCloseCallback(handle, [](GLFWwindow* native)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onClose)
			handle->onClose();
	});

	glfwSetWindowFocusCallback(handle, [](GLFWwindow* native, int focused)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onFocus)
			handle->onFocus(static_cast<bool>(focused));
	});


	glfwSetKeyCallback(handle, [](GLFWwindow* native, int key, int scancode, int action, int mods)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onKey)
			handle->onKey(action, static_cast<Key>(key));
	});


	glfwSetScrollCallback(handle, [](GLFWwindow* native, double xOffset, double yOffset)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onScroll)
			handle->onScroll({xOffset, yOffset});
	});

	glfwSetCursorPosCallback(handle, [](GLFWwindow* native, double x, double y)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onMouseMove)
			handle->onMouseMove({x, y});
	});

	glfwSetCursorEnterCallback(handle, [](GLFWwindow* native, int entered)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onMouseEnter)
			handle->onMouseEnter(entered);
	});

	glfwSetMouseButtonCallback(handle, [](GLFWwindow* native, int button, int action, int mods)
	{
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onMouse)
			handle->onMouse(action, static_cast<Mouse>(button));
	});


	glfwSetDropCallback(handle, [](GLFWwindow* native, int pathCount, const char* paths[]) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onDrop)
			handle->onDrop(pathCount, paths);
	});
}

void Window::init()
{
	if(!glfwInit())
		throw std::runtime_error("GLFW init failed");

	Monitor::init();

	if(Renderer::get()->getAPI() == Renderer::API::Vulkan)
	{
		if(!glfwVulkanSupported())
			throw std::runtime_error("GLFW: Vulkan not supported");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // 创建新窗口默认不可见
	// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // 启用无边框

	/*
	const auto& monitor = Monitor::getPrimary();
	auto mode = glfwGetVideoMode(static_cast<GLFWmonitor*>(monitor.getNativeHandle()));
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
	*/
}

void Window::deinit()
{
	Monitor::deinit();
	glfwTerminate();
}
