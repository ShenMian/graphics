// Copyright 2021 ShenMian
// License(Apache-2.0)

#include "UI.h"
#include "Core/Window.h"
#include "Renderer.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_vulkan.h>

void UI::update()
{
	ImGui::ShowDemoWindow();
}

void UI::init(const Window& win)
{
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // 启用 Docking

	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(win.getNativeHandle()), true);
		ImGui_ImplOpenGL3_Init("#version 450");
		break;

	case Vulkan:
		ImGui_ImplGlfw_InitForVulkan(reinterpret_cast<GLFWwindow*>(win.getNativeHandle()), true);
		// ImGui_ImplVulkan_Init();
		break;
	}
}

void UI::deinit()
{
	switch(Renderer::getAPI())
	{
		using enum Renderer::API;

	case OpenGL:
		ImGui_ImplOpenGL3_Shutdown();
		break;

	case Vulkan:
		ImGui_ImplVulkan_Shutdown();
		break;
	}

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
