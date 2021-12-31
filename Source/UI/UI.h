// Copyright 2021 ShenMian
// License(Apache-2.0)

#pragma once

class Window;

class UI
{
public:
	static void begin();
	static void end();

	static void init(const Window& win);
	static void deinit();
};
