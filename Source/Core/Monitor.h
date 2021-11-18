// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include <Math/Vector2.hpp>
#include <vector>

struct GLFWmonitor;

/**
 * @brief 显示器.
 */
class Monitor
{
public:
    /**
     * @brief 获取主显示器.
     */
    static const Monitor& getPrimary();

    /**
     * @brief 获取全部显示器.
     */
    static const std::vector<Monitor>& getMonitors();

    /**
     * @brief 获取显示器名称.
     */
    const std::string& getName() const;

    /**
     * @brief 获取显示器大小(即分辨率), 单位: 像素.
     */
    Vector2i getSize() const;

    /**
     * @brief 获取显示器刷新率, 单位: Hz.
     */
    int getRefreshRate() const;

    /**
     * @brief 是否为主显示器.
     */
    bool isPrimary() const;

	/**
	 * @brief 获取本地句柄.
	 */
    void* getNativeHandle() const;

    static void init();
    static void deinit();

private:
    Monitor() = default;
    Monitor(GLFWmonitor* handle);

    GLFWmonitor* handle = nullptr;
    std::string  name;

    static Monitor              primary;
    static std::vector<Monitor> monitors;

    static void update();
};
