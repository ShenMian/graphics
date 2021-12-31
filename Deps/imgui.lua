project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	files {
        "imgui/**.h",
        "imgui/*.cpp",

        "imgui/backends/imgui_impl_glfw.cpp",
        "imgui/backends/imgui_impl_opengl3.cpp",
        }-- "imgui/backends/imgui_impl_vulkan.cpp"}

	includedirs {
        "imgui",
		"%{deps_inc.glad}",
		"%{deps_inc.glfw}",
		"%{deps_inc.vulkan}"}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
