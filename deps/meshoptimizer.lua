project "meshoptimizer"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

    targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/lib")
    objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

	files {
		"meshoptimizer/include/**.h",
		"meshoptimizer/src/**.cpp"}

	includedirs "meshoptimizer/include"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
