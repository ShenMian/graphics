﻿// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Core/Window.h"
#include <string>

class Renderer
{
public:
	enum class API
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
	 * @brief 设置图形 API.
	 *
	 * @param api 图形 API.
	 */
	static void setAPI(API api);

	/**
	 * @brief 获取图形 API.
	 */
	static API getAPI();

	virtual std::string getDeviceName() const   = 0;
	virtual std::string getRendererName() const = 0;
	virtual std::string getVendorName() const   = 0;

	static void init(const Window& win);
	static void deinit();

private:
	static API api;
};
