// Copyright 2021 SMS
// License(Apache-2.0)

#include "Window.h"
#include <cassert>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Window::Window(const std::string& title, Vector2i size)
{
    auto       monitor   = glfwGetPrimaryMonitor();
    const auto videoMode = glfwGetVideoMode(monitor);

    if(false)
        handle = glfwCreateWindow(videoMode->width, videoMode->height, title.c_str(), monitor, nullptr);
    else
        handle = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);

    glfwMakeContextCurrent(handle);

    auto ret = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    assert(ret);

    glfwSetWindowUserPointer(handle, static_cast<void*>(this));

    setupCallbacks();

    // glfwSetInputMode(handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // ¿ªÆô MASS ¿¹¾â³Ý
    // glfwWindowHint(GLFW_SAMPLES, 2);
    // glEnable(GL_MULTISAMPLE);
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

Vector2i Window::getSize() const
{
    int x, y;
    glfwGetWindowSize(handle, &x, &y);
    return {x, y};
}

void Window::setPosition(Vector2i pos)
{
    glfwSetWindowPos(handle, pos.x, pos.y);
}

Vector2i Window::getPosition()
{
    Vector2i size;
    glfwGetWindowPos(handle, &size.x, &size.y);
    return size;
}

void Window::setVisible(bool visible)
{
    if(visible)
        glfwShowWindow(handle);
    else
        glfwHideWindow(handle);
}

bool Window::isVisible() const
{
    return glfwGetWindowAttrib(handle, GLFW_VISIBLE);
}

void Window::setSync(bool enable)
{
    if(enable)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
}

void* Window::getNativeHandle() const
{
    return handle;
}

void Window::init()
{
    auto ret = glfwInit();
    assert(ret && "GLFW init failed");
}

void Window::deinit()
{
    glfwTerminate();
}

void Window::setupCallbacks()
{
    glfwSetErrorCallback([](int error, const char* desc)
    {
        throw std::exception(desc);
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
            handle->onKey(action, key);
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
