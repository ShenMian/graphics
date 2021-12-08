project "glslang"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

	files {}

	includedirs "glslang"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
