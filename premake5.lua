-- Copyright 2021 SMS
-- License(Apache-2.0)

include "ThirdParty/premake/solution_items.lua"

workspace "Graphics"
    architecture "x86_64"
    startproject "Triangle"
    configurations {"Debug", "Release"}
    flags "MultiProcessorCompile"

    solution_items {
        ".editorconfig",
        "README.md",
        "premake5.lua"}

    filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NDEBUG"
        runtime "Release"
        optimize "on"

    filter "system:linux"
        linkoptions "-pthread"

deps = {}
deps["stb"]           = "%{wks.location}/ThirdParty/stb"
deps["math"]          = "%{wks.location}/ThirdParty/Math"
deps["glad"]          = "%{wks.location}/ThirdParty/glad"
deps["glfw"]          = "%{wks.location}/ThirdParty/glfw"
deps["assimp"]        = "%{wks.location}/ThirdParty/assimp"
deps["shaderc"]       = "%{wks.location}/ThirdParty/shaderc"
deps["glslang"]       = "%{wks.location}/ThirdParty/glslang"
deps["vulkan"]        = "%{wks.location}/ThirdParty/Vulkan-Headers"
deps["spirv_tools"]   = "%{wks.location}/ThirdParty/SPIRV-Tools"
deps["spirv_headers"] = "%{wks.location}/ThirdParty/SPIRV-Headers"
deps["meshoptimizer"] = "%{wks.location}/ThirdParty/meshoptimizer"

deps_include = {}
deps_include["stb"]           = "%{deps.stb}"
deps_include["math"]          = "%{deps.math}/include"
deps_include["glad"]          = "%{deps.glad}/include"
deps_include["glfw"]          = "%{deps.glfw}/include"
deps_include["assimp"]        = "%{deps.assimp}/include"
-- deps_include["shaderc"] = "%{deps.shaderc}/libshaderc/include"
deps_include["vulkan"]        = "%{deps.vulkan}/include"
deps_include["meshoptimizer"] = "%{deps.meshoptimizer}/src"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Source"

group "Examples"
    include "Examples/Triangle"
    include "Examples/Test"
group ""

group "ThirdParty"
    include "ThirdParty/glad"
    include "ThirdParty/glfw"
    include "ThirdParty/assimp"
    -- include "ThirdParty/shaderc"
    -- include "ThirdParty/glslang"
    -- include "ThirdParty/spirv_tools"
    include "ThirdParty/meshoptimizer"
group ""
