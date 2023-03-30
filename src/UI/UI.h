// Copyright 2022 ShenMian
// License(Apache-2.0)

#pragma once

class Window;

class UI
{
public:
	static void begin_frame();
	static void end_frame();

	static void init(const Window& win);
	static void deinit();
};
