-- Copyright 2021 SMS
-- License(Apache-2.0)

project("Graphics")
kind("StaticLib")
language("C++")
cppdialect("C++latest")
staticruntime("on")

targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/lib")
objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

defines({ "_CRT_SECURE_NO_WARNINGS" }) -- stb_image_write.h

files({ "**.cpp", "**.hpp", "**.h", "**.inl", "CMakeLists.txt", "premake5.lua" })

sysincludedirs({
	"%{deps_inc.fmt}",
	"%{deps_inc.stb}",
	"%{deps_inc.math}",
	"%{deps_inc.glad}",
	"%{deps_inc.glfw}",
	"%{deps_inc.imgui}",
	"%{deps_inc.assimp}",
	"%{deps_inc.vulkan}",
	"%{deps_inc.imguizmo}",
	"%{deps_inc.vk_bootstrap}",
	"%{deps_inc.meshoptimizer}",
	"%{deps_inc.imgui_node_editor}",
})

includedirs("%{wks.location}/src")

links({
	"glad",
	"glfw",
	"imgui",
	"assimp",
	"imguizmo",
	"vk_bootstrap",
	"meshoptimizer",
	"%{deps_lib.vulkan}",
	"imgui_node_editor",
})
-- "shaderc"
