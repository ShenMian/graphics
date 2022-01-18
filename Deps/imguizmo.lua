project "imguizmo"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

    targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/lib")
    objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

	files {
		"ImGuizmo/*.h",
		"ImGuizmo/*.cpp"}

	includedirs "ImGuizmo"
	sysincludedirs "%{deps_inc.imgui}"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
