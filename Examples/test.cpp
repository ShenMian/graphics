// Copyright 2021 SMS
// License(Apache-2.0)

#include "Model.h"

#include <conio.h>

int main()
{
	Model model;
	model.loadAsync("../../3DModel/scene/SunTemple/SunTemple.fbx", [](std::error_code ec)
	{
		if(ec)
		{
			puts(ec.message().c_str());
			return;
		}
		puts("载入成功 SunTemple");
	});

	getch();

	while(true)
		;

	return 0;
}