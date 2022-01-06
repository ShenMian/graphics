-- Copyright 2021 SMS
-- License(Apache-2.0)

project "cube"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/bin")
	objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

	files {"**.cpp", "**.hpp", "**.h", "**.inl", "CMakeLists.txt", "premake5.lua", "Shaders/**"}

	includedirs {
		"%{wks.location}/Source",
		"%{deps_inc.math}",
		"%{deps_inc.assimp}",
		"%{deps_inc.vulkan}"}

	links "Graphics"

	postbuildcommands "{COPYDIR} \"Shaders\" \"%{cfg.targetdir}\""
