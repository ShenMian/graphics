-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Graphics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/lib")
	objdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/obj")

	defines {"_CRT_SECURE_NO_WARNINGS"} -- stb_image_write.h

	files {"**.cpp", "**.hpp", "**.h", "**.inl", "premake5.lua"}

	includedirs {
		"%{wks.location}/Source",
		"%{deps_include.stb}",
		"%{deps_include.math}",
		"%{deps_include.glad}",
		"%{deps_include.glfw}",
		"%{deps_include.assimp}",
		"%{deps_include.vulkan}",
		"%{deps_include.meshoptimizer}"}

	links {
		"glad",
		"glfw",
		"assimp",
		"meshoptimizer",
		-- "shaderc"
		-- "%{deps.vulkan}/lib/vulkan-1",
		}
