-- Copyright 2021 SMS
-- License(Apache-2.0)

include "ThirdParty/premake/solution_items.lua"

workspace "Graphics"
    architecture "x86_64"
    -- startproject ""
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

thirdparty = {}
thirdparty["stb"] = "%{wks.location}/ThirdParty/stb"
thirdparty["math"] = "%{wks.location}/ThirdParty/Math"
thirdparty["glad"] = "%{wks.location}/ThirdParty/glad"
thirdparty["glfw"] = "%{wks.location}/ThirdParty/glfw"
thirdparty["assimp"] = "%{wks.location}/ThirdParty/assimp"
thirdparty["shaderc"] = "%{wks.location}/ThirdParty/shaderc"
thirdparty["glslang"] = "%{wks.location}/ThirdParty/glslang"
thirdparty["vulkan"] = "%{wks.location}/ThirdParty/Vulkan-Headers"
thirdparty["spirv_tools"] = "%{wks.location}/ThirdParty/SPIRV-Tools"
thirdparty["spirv_headers"] = "%{wks.location}/ThirdParty/SPIRV-Headers"
thirdparty["meshoptimizer"] = "%{wks.location}/ThirdParty/meshoptimizer"

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
