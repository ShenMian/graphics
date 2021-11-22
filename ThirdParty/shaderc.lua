project "shaderc"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

	defines "ENABLE_HLSL"

	files {
		"shaderc/libshaderc/src/shaderc.cc",
		"shaderc/libshaderc_util/src/compiler.cc",
		"shaderc/libshaderc_util/src/spirv_tools_wrapper.cc"}

	includedirs {
		"shaderc/libshaderc/include",
		"shaderc/libshaderc_util/include",
		"%{thirdparty.glslang}",
		"%{thirdparty.spirv_tools}/include",
		"%{thirdparty.spirv_headers}/include"}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	links {
		--"glslang",
		"spirv_tools",}
		--"spirv-headers"}
