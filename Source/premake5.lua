-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Graphics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	defines {"_CRT_SECURE_NO_WARNINGS"} -- stb_image_write.h

	files {"**.cpp", "**.hpp", "**.h", "**.inl", "CMakeLists.txt", "premake5.lua"}

	sysincludedirs {
		"%{deps_inc.stb}",
		"%{deps_inc.math}",
		"%{deps_inc.glad}",
		"%{deps_inc.glfw}",
		"%{deps_inc.assimp}",
		"%{deps_inc.vulkan}",
		"%{deps_inc.meshoptimizer}"}

	includedirs {
		"%{wks.location}/Source"}

	links {
		"glad",
		"glfw",
		"assimp",
		"meshoptimizer",
		-- "shaderc"
		-- "%{deps.vulkan}/lib/vulkan-1",
		}
