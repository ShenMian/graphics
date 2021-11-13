-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Source"
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
		"%{thirdparty.stb}",
		"%{thirdparty.math}/include",
		"%{thirdparty.glad}/include",
		"%{thirdparty.glfw}/include",
		"%{thirdparty.assimp}/include",
		-- "%{thirdparty.shaderc}/libshaderc/include",
		"%{thirdparty.vulkan}/include",
		"%{thirdparty.meshoptimizer}/src"}

	links {
		"glad",
		"glfw",
		"assimp",
		"meshoptimizer",
		-- "shaderc"
		-- "%{thirdparty.vulkan}/lib/vulkan-1",
		}
