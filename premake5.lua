-- Copyright 2021 SMS
-- License(Apache-2.0)

include("deps/premake/solution_items.lua")

workspace("Graphics")
architecture("x86_64")
startproject("Triangle")
configurations({ "Debug", "Release" })
flags("MultiProcessorCompile")

output_dir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

configurations({ "Debug", "Release" })

filter("configurations:Debug")
defines("DEBUG")
runtime("Debug")
symbols("on")

filter("configurations:Release")
defines("NDEBUG")
runtime("Release")
optimize("on")

filter("system:linux")
linkoptions("-pthread")

solution_items({
	".editorconfig",
	"README.md",
	"CMakeLists.txt",
	"premake5.lua",
})

-- 启用 OpenMP
-- openmp "On"
-- filter "toolset:not msc*"
--     buildoptions "-fopenmp"

-- 第三方库路径
deps = {}
deps["fmt"] = "%{wks.location}/deps/fmt"
deps["stb"] = "%{wks.location}/deps/stb"
deps["math"] = "%{wks.location}/deps/Math"
deps["glad"] = "%{wks.location}/deps/glad"
deps["glfw"] = "%{wks.location}/deps/glfw"
deps["imgui"] = "%{wks.location}/deps/imgui"
deps["assimp"] = "%{wks.location}/deps/assimp"
deps["shaderc"] = "%{wks.location}/deps/shaderc"
deps["glslang"] = "%{wks.location}/deps/glslang"
deps["imguizmo"] = "%{wks.location}/deps/ImGuizmo"
deps["spirv_tools"] = "%{wks.location}/deps/SPIRV-Tools"
deps["vk_bootstrap"] = "%{wks.location}/deps/vk-bootstrap"
deps["spirv_headers"] = "%{wks.location}/deps/SPIRV-Headers"
deps["meshoptimizer"] = "%{wks.location}/deps/meshoptimizer"
deps["imgui_node_editor"] = "%{wks.location}/deps/imgui-node-editor"

-- 第三方库头文件路径
deps_inc = {}
deps_inc["fmt"] = "%{deps.fmt}/include"
deps_inc["stb"] = "%{deps.stb}"
deps_inc["math"] = "%{deps.math}/include"
deps_inc["glad"] = "%{deps.glad}/include"
deps_inc["glfw"] = "%{deps.glfw}/include"
deps_inc["imgui"] = "%{deps.imgui}"
deps_inc["assimp"] = "%{deps.assimp}/include"
deps_inc["vulkan"] = "%{wks.location}/deps/Vulkan-Headers/include"
deps_inc["imguizmo"] = "%{wks.location}/deps/ImGuizmo"
deps_inc["vk_bootstrap"] = "%{deps.vk_bootstrap}/src"
deps_inc["meshoptimizer"] = "%{deps.meshoptimizer}/src"
deps_inc["imgui_node_editor"] = "%{deps.imgui_node_editor}"
-- deps_inc["shaderc"] = "%{deps.shaderc}/libshaderc/include"

deps_lib = {}
deps_lib["vulkan"] = "%{wks.location}/deps/VulkanSDK/Lib/vulkan-1"

include("src")

group("examples")
include("Examples/Triangle")
include("Examples/Cube")
include("Examples/Viewer")
group("")

group("deps")
include("deps/glad")
include("deps/glfw")
include("deps/imgui")
include("deps/assimp")
include("deps/imguizmo")
include("deps/vk_bootstrap")
include("deps/meshoptimizer")
include("deps/imgui_node_editor")
-- include "deps/shaderc"
-- include "deps/glslang"
-- include "deps/spirv_tools"
-- include "deps/spirv_cross"
group("")
