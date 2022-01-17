-- Copyright 2021 SMS
-- License(Apache-2.0)

project "test"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/bin")
	objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

	files {"**.cpp", "**.hpp", "**.h", "**.inl", "CMakeLists.txt", "premake5.lua", "Shaders/**"}

	includedirs {
		"%{wks.location}/Source",
		"%{deps_inc.glad}",
		"%{deps_inc.math}",
		"%{deps_inc.imgui}",
		"%{deps_inc.imgui_node_editor}",
		"%{deps_inc.vulkan}"}

	links "Graphics"

	postbuildcommands "{COPYDIR} \"Shaders\" \"%{cfg.targetdir}\""
