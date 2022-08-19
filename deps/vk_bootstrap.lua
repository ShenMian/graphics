project "vk_bootstrap"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	warnings "Off"

    targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/lib")
    objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

	files {
		"vk-bootstrap/src/**.h",
		"vk-bootstrap/src/**.cpp"}

	includedirs {
        "vk-bootstrap/src",
		"%{deps_inc.vulkan}"}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
