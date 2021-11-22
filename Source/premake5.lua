-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Graphics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	defines {"_CRT_SECURE_NO_WARNINGS"} -- stb_image_write.h

	files {"**.cpp", "**.hpp", "**.h", "**.inl", "premake5.lua"}

	includedirs {
		"%{wks.location}/Source",
		"%{deps_inc.stb}",
		"%{deps_inc.math}",
		"%{deps_inc.glad}",
		"%{deps_inc.glfw}",
		"%{deps_inc.assimp}",
		"%{deps_inc.vulkan}",
		"%{deps_inc.meshoptimizer}"}

	links {
		"glad",
		"glfw",
		"assimp",
		"meshoptimizer",
		-- "shaderc"
		-- "%{deps.vulkan}/lib/vulkan-1",
		}
