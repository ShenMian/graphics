project "assimp"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/lib")
	objdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/obj")

	files
	{
		"assimp/include/**.h",
		"assimp/include/**.hpp",
		"assimp/code/**.c",
		"assimp/code/**.cpp",
	}

	includedirs {
    "assimp/include",
    "assimp/code",
		"assimp",
    "assimp/contrib",
    "assimp/contrib/zlib",
    "assimp/contrib/rapidjson/include",
    "assimp/contrib/pugixml/src",
    "assimp/contrib/unzip",
    "assimp/contrib/",
    "assimp/contrib/",}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
