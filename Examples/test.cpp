// Copyright 2021 SMS
// License(Apache-2.0)

#include "Model.h"

#include <conio.h>

int main()
{
	/*
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}
	*/

	Model model;
	model.load("../../3DModel/scene/SunTemple/SunTemple.fbx");
	puts("载入成功 SunTemple");

	getch();
	return 0;
}