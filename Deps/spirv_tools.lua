project "spirv_tools"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

	files {"SPIRV-Tools/source/binary.cpp"}

	includedirs {
		"SPIRV-Tools",
		"SPIRV-Tools/include",
		"%{thirdparty.spirv_headers}/include"}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
