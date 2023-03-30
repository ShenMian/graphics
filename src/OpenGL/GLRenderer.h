// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

#include "Renderer.h"

class GLRenderer : public Renderer
{
public:
	std::string get_device_name() const override;
	std::string get_renderer_name() const override;
	std::string get_vendor_name() const override;

	/**
	 * @note 将会在调用线程中使用指定窗口的 context.
	 */
	static void init(const Window& win);
	static void deinit();

private:
	static void setup_debug_callback();
};
