project "imgui_node_editor"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

    targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/lib")
    objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

	files {
		-- "imgui-node-editor/external/imgui/*.h",
		-- "imgui-node-editor/external/imgui/*.cpp",
		"imgui-node-editor/*.h",
		"imgui-node-editor/*.inl",
		"imgui-node-editor/*.cpp"}

	includedirs "imgui-node-editor"
	sysincludedirs {
		"%{deps_inc.imgui}"
		-- "imgui-node-editor/external/imgui"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
