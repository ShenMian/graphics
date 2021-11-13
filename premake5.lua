-- Copyright 2021 SMS
-- License(Apache-2.0)

include "Thirdparty/premake/solution_items.lua"

workspace "Graphics"
    architecture "x86_64"
    -- startproject ""
    configurations {"Debug", "Release"}
    flags "MultiProcessorCompile"

    solution_items {
        ".clang-format",
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
thirdparty["stb"] = "%{wks.location}/Thirdparty/stb"
thirdparty["math"] = "%{wks.location}/Thirdparty/Math"
thirdparty["glad"] = "%{wks.location}/Thirdparty/glad"
thirdparty["glfw"] = "%{wks.location}/Thirdparty/glfw"
thirdparty["assimp"] = "%{wks.location}/Thirdparty/assimp"
thirdparty["shaderc"] = "%{wks.location}/Thirdparty/shaderc"
thirdparty["glslang"] = "%{wks.location}/Thirdparty/glslang"
thirdparty["vulkan"] = "%{wks.location}/Thirdparty/Vulkan-Headers"
thirdparty["spirv_tools"] = "%{wks.location}/Thirdparty/SPIRV-Tools"
thirdparty["spirv_headers"] = "%{wks.location}/Thirdparty/SPIRV-Headers"
thirdparty["meshoptimizer"] = "%{wks.location}/Thirdparty/meshoptimizer"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Source"

group "Examples"
    include "Examples/Triangle"
    include "Examples/Test"
group ""

group "Thirdparty"
    include "Thirdparty/glad"
    include "Thirdparty/glfw"
    include "Thirdparty/assimp"
    -- include "Thirdparty/shaderc"
    -- include "Thirdparty/glslang"
    -- include "Thirdparty/spirv_tools"
    include "Thirdparty/meshoptimizer"
group ""
