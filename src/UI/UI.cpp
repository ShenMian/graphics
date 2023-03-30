// Copyright 2022 ShenMian
// License(Apache-2.0)

#include "UI.h"
#include "Core/Window.h"
#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_vulkan.h>

#include <ImGuizmo.h>

void UI::beginFrame()
{
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		ImGui_ImplOpenGL3_NewFrame();
		break;

	case Vulkan:
		ImGui_ImplVulkan_NewFrame();
		break;
	}
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void UI::endFrame()
{
	ImGui::Render();
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		break;

	case Vulkan:
		// ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData());
		break;
	}

	if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		const auto context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(context);
	}
}

void UI::init(const Window& win)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // 启用 Docking

	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

	case OpenGL:
		ImGui_ImplGlfw_InitForOpenGL(win.getHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 450");
		break;

	case Vulkan:
		ImGui_ImplGlfw_InitForVulkan(win.getHandle(), true);
		// ImGui_ImplVulkan_Init(ImGui_ImplVulkan_InitInfo);
		break;
	}
}

void UI::deinit()
{
	switch(Renderer::getBackend())
	{
		using enum Renderer::Backend;

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
