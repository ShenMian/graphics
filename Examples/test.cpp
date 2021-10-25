// Copyright 2021 SMS
// License(Apache-2.0)

#include "Graphics.h"

int main()
{
	Window::init();

	Window window("Title", {1920 / 2, 1080 / 2});

	while(true)
		window.update();

	return 0;
}