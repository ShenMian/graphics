// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "Window.h"
#include "Image.h"
#include "Monitor.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <cassert>
#include <stdexcept>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

namespace fs = std::filesystem;

Window::Window(std::string_view title, const Vector2i& size)
{
	handle_ = glfwCreateWindow(size.x(), size.y(), title.data(), nullptr, nullptr);
	if(handle_ == nullptr)
		throw std::runtime_error("failed to create window");

	glfwSetWindowUserPointer(handle_, static_cast<void*>(this));

	setup_callbacks();
}

Window::~Window()
{
	glfwDestroyWindow(handle_);
}

void Window::update()
{
	if(Renderer::get_backend() == Renderer::Backend::OpenGL)
		glfwSwapBuffers(handle_);
	glfwPollEvents();
}

void Window::set_title(std::string_view title)
{
	glfwSetWindowTitle(handle_, title.data());
}

void Window::set_size(const Vector2i& size)
{
	glfwSetWindowSize(handle_, size.x(), size.y());
}

Vector2i Window::get_size() const
{
	int x, y;
	glfwGetWindowSize(handle_, &x, &y);
	return {x, y};
}

void Window::set_position(const Vector2i& pos)
{
	glfwSetWindowPos(handle_, pos.x(), pos.y());
}

Vector2i Window::get_position() const
{
	int x, y;
	glfwGetWindowPos(handle_, &x, &y);
	return {x, y};
}

void Window::set_visible(bool visible)
{
	if(visible)
		glfwShowWindow(handle_);
	else
		glfwHideWindow(handle_);
}

bool Window::is_visible() const noexcept
{
	return glfwGetWindowAttrib(handle_, GLFW_VISIBLE);
}

void Window::set_fullscreen(bool fullscreen)
{
	if(fullscreen)
	{
		// 保存窗口状态, 便于复原
		this->position_ = get_position();
		this->size_     = get_size();

		const auto monitor = Monitor::get_primary();
		glfwSetWindowMonitor(handle_, monitor->get_handle(), 0, 0, monitor->get_size().x(), monitor->get_size().y(),
		                     GLFW_DONT_CARE);
	}
	else
		glfwSetWindowMonitor(handle_, nullptr, position_.x(), position_.y(), size_.x(), size_.y(), GLFW_DONT_CARE);
}

bool Window::is_fullscreen() const noexcept
{
	return glfwGetWindowMonitor(handle_) != nullptr;
}

void Window::set_vsync(bool enable) noexcept
{
	glfwMakeContextCurrent(handle_);
	if(enable)
		glfwSwapInterval(1);
	else
		glfwSwapInterval(0);
}

void Window::set_resizable(bool enable)
{
	glfwSetWindowAttrib(handle_, GLFW_RESIZABLE, enable);
}

bool Window::is_resizable() const noexcept
{
	return glfwGetWindowAttrib(handle_, GLFW_RESIZABLE);
}

void Window::set_floating(bool enable)
{
	glfwSetWindowAttrib(handle_, GLFW_FLOATING, enable);
}

bool Window::is_floating() const noexcept
{
	return glfwGetWindowAttrib(handle_, GLFW_FLOATING);
}

void Window::set_icon(const Image& image)
{
	if(image.size().x() > 48 && image.size().y() > 48)
		throw std::runtime_error("image size is too large");

	GLFWimage glfwImage;
	glfwImage.pixels = const_cast<unsigned char*>(image.data());
	glfwImage.width  = image.size().x();
	glfwImage.height = image.size().y();
	glfwSetWindowIcon(handle_, 1, &glfwImage);
}

void Window::request_focus()
{
	glfwRequestWindowAttention(handle_);
}

void Window::set_cursor_lock(bool enable)
{
	glfwSetInputMode(handle_, GLFW_CURSOR, enable ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool Window::is_cursor_lock() const
{
	return glfwGetInputMode(handle_, GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}

void Window::set_cursor_position(const Vector2i& pos)
{
	glfwSetCursorPos(handle_, pos.x(), pos.y());
}

Vector2d Window::get_cursor_position() const
{
	double x, y;
	glfwGetCursorPos(handle_, &x, &y);
	return {x, y};
}

void Window::set_raw_mouse_motion(bool enable)
{
	if(glfwRawMouseMotionSupported())
		glfwSetInputMode(handle_, GLFW_RAW_MOUSE_MOTION, enable);
}

GLFWwindow* Window::get_handle() const
{
	return handle_;
}

void Window::setup_callbacks()
{
	glfwSetErrorCallback([](int error, const char* desc) {
		if(error == 65537)
			throw std::runtime_error("GLFW: some windows has not been destroyed");
		throw std::runtime_error(fmt::format("GLFW: {}", desc));
	});

	glfwSetWindowSizeCallback(handle_, [](GLFWwindow* native, int width, int height) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onResize)
			handle->onResize({width, height});
	});

	glfwSetWindowCloseCallback(handle_, [](GLFWwindow* native) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onClose)
			handle->onClose();
	});

	glfwSetWindowFocusCallback(handle_, [](GLFWwindow* native, int focused) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onFocus)
			handle->onFocus(static_cast<bool>(focused));
	});


	glfwSetKeyCallback(handle_, [](GLFWwindow* native, int key, int scancode, int action, int mods) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onKey)
			handle->onKey(action, static_cast<Key>(key));
	});


	glfwSetScrollCallback(handle_, [](GLFWwindow* native, double xOffset, double yOffset) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onScroll)
			handle->onScroll({xOffset, yOffset});
	});

	glfwSetCursorPosCallback(handle_, [](GLFWwindow* native, double x, double y) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onMouseMove)
			handle->onMouseMove({x, y});
	});

	glfwSetCursorEnterCallback(handle_, [](GLFWwindow* native, int entered) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onMouseEnter)
			handle->onMouseEnter(entered);
	});

	glfwSetMouseButtonCallback(handle_, [](GLFWwindow* native, int button, int action, int mods) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onMouse)
			handle->onMouse(action, static_cast<Mouse>(button));
	});


	glfwSetDropCallback(handle_, [](GLFWwindow* native, int pathCount, const char* paths[]) {
		const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
		if(handle->onDrop)
			handle->onDrop(pathCount, paths);
	});
}

void Window::init()
{
	if(!glfwInit())
		throw std::runtime_error("GLFW: failed to init");

	Monitor::init();

	if(Renderer::get()->get_backend() == Renderer::Backend::Vulkan)
	{
		if(!glfwVulkanSupported())
			throw std::runtime_error("GLFW: Vulkan not supported");
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // 创建新窗口默认不可见
	glfwWindowHint(GLFW_SAMPLES, 2);          // 开启 MSAA 抗锯齿

	// glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // 启用无边框

	/*
	const auto& monitor = Monitor::getPrimary();
	auto mode = glfwGetVideoMode(static_cast<GLFWmonitor*>(monitor.getHandle()));
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
