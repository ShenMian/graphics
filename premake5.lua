-- Copyright 2021 SMS
-- License(Apache-2.0)

include "ThirdParty/premake/solution_items.lua"

workspace "Graphics"
    architecture "x86_64"
    startproject "Triangle"
    flags "MultiProcessorCompile"

    output_dir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"
    targetdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/lib")
    objdir("%{wks.location}/build/" .. output_dir .. "/%{prj.name}/obj")

    configurations {"Debug", "Release"}

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

    solution_items {
        ".editorconfig",
        "README.md",
        "CMakeLists.txt",
        "premake5.lua"}

    -- 启用 OpenMP
    openmp "On"
    filter "toolset:not msc*"
        buildoptions "-fopenmp"

    -- 第三方库路径
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

    -- 第三方库头文件路径
    deps_inc = {}
    deps_inc["stb"]           = "%{deps.stb}"
    deps_inc["math"]          = "%{deps.math}/include"
    deps_inc["glad"]          = "%{deps.glad}/include"
    deps_inc["glfw"]          = "%{deps.glfw}/include"
    deps_inc["assimp"]        = "%{deps.assimp}/include"
    -- deps_inc["shaderc"] = "%{deps.shaderc}/libshaderc/include"
    deps_inc["vulkan"]        = "%{deps.vulkan}/include"
    deps_inc["meshoptimizer"] = "%{deps.meshoptimizer}/src"

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
        -- include "ThirdParty/spirv_cross"
        include "ThirdParty/meshoptimizer"
    group ""
