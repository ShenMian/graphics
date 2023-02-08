// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

class Window;

class UI
{
public:
	static void beginFrame();
	static void endFrame();

	static void init(const Window& win);
	static void deinit();
};
