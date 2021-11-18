// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "Window.h"
#include "Image.h"
#include "Monitor.h"
#include <cassert>
#include <format>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace fs = std::filesystem;

Window::Window(const std::string& title, Vector2i size, bool fullscreen)
{
    handle = glfwCreateWindow(size.x, size.y, title.c_str(),
        fullscreen ? reinterpret_cast<GLFWmonitor*>(Monitor::getPrimary().getNativeHandle()) : nullptr, nullptr);

    // TODO: OpenGL/Glad 相关代码, 转移到合适的位置
    glfwMakeContextCurrent(handle);
    static auto ret = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if(!ret)
        throw std::exception("glad init failed");

    glfwSetWindowUserPointer(handle, static_cast<void*>(this));

    // 将光标锁定在窗口内
    // glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    glfwSwapBuffers(handle);
    glfwPollEvents();
}

void Window::setTitle(const std::string& title)
{
    glfwSetWindowTitle(handle, title.c_str());
}

void Window::setSize(Vector2i size)
{
    glfwSetWindowSize(handle, size.x, size.y);
}

Vector2i Window::getSize() const noexcept
{
    int x, y;
    glfwGetWindowSize(handle, &x, &y);
    return {x, y};
}

void Window::setPosition(Vector2i pos)
{
    glfwSetWindowPos(handle, pos.x, pos.y);
}

Vector2i Window::getPosition() noexcept
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
        size     = getSize();
        position = getPosition();
    }

	auto monitor = glfwGetPrimaryMonitor();
    glfwSetWindowMonitor(handle, fullscreen ? monitor : nullptr, position.x, position.y, size.x, size.y, 0);
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
        throw std::exception("image size too large");

    GLFWimage glfwImage;
    glfwImage.pixels = const_cast<unsigned char*>(image.getData());
    glfwImage.width  = image.getSize().x;
    glfwImage.height = image.getSize().y;
    glfwSetWindowIcon(handle, 1, &glfwImage);
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
            assert(false && "ensure that all windows created on the stack are destroyed");
        throw std::exception(std::format("GLFW error: {}", desc).c_str());
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

    glfwSetCursorPosCallback(handle, [](GLFWwindow* native, double x, double y)
    {
        const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
        if(handle->onMouseMove)
            handle->onMouseMove(x, y);
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
            handle->onScroll(xOffset, yOffset);
    });

    glfwSetMouseButtonCallback(handle, [](GLFWwindow* native, int button, int action, int mods)
    {
        const auto handle = static_cast<Window*>(glfwGetWindowUserPointer(native));
        // TODO
    });
}

void Window::init()
{
    auto ret = glfwInit();
    if(!ret)
        throw std::exception("GLFW init failed");
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // 创建新窗口默认不可见
    Monitor::init();
}

void Window::deinit()
{
    Monitor::deinit();
    glfwTerminate();
}
