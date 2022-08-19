// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

#include "Renderer.h"

class GLRenderer : public Renderer
{
public:
	std::string getDeviceName() const override;
	std::string getRendererName() const override;
	std::string getVendorName() const override;

	/**
	 * @note 将会在调用线程中使用指定窗口的 context.
	 */
	static void init(const Window& win);
	static void deinit();
};
