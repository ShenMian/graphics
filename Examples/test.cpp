// Copyright 2021 SMS
// License(Apache-2.0)

#include "Scene.h"

#include <conio.h>

int main()
{
	Model model;
	model.loadAsync("../../3DModel/scene/Crytek_Sponza/sponza.obj", [](std::error_code ec)
	{
		if(ec)
		{
			puts(ec.message().c_str());
			return;
		}
		puts("载入成功 sponza");
	});

	while(true)
		;

	return 0;
}