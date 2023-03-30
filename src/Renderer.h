// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Core/Window.h"
#include <string>

class Renderer
{
public:
	enum class Backend
	{
		OpenGL,
		Vulkan,
		// D3D12,
		// Metal
	};

	/**
	 * @brief 获取单例.
	 */
	static Renderer* get();

	/**
	 * @brief 设置图形后端.
	 *
	 * @param backend 图形后端.
	 */
	static void setBackend(Backend backend);

	/**
	 * @brief 获取图形后端.
	 */
	static Backend getBackend();

	virtual std::string getDeviceName() const   = 0;
	virtual std::string getRendererName() const = 0;
	virtual std::string getVendorName() const   = 0;

	static void init(const Window& win);
	static void deinit();

private:
	static Backend backend;
};
