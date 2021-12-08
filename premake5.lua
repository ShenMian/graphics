-- Copyright 2021 SMS
-- License(Apache-2.0)

include "Deps/premake/solution_items.lua"

workspace "Graphics"
    architecture "x86_64"
    startproject "Triangle"
    configurations {"Debug", "Release"}
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
    deps["stb"]           = "%{wks.location}/Deps/stb"
    deps["math"]          = "%{wks.location}/Deps/Math"
    deps["glad"]          = "%{wks.location}/Deps/glad"
    deps["glfw"]          = "%{wks.location}/Deps/glfw"
    deps["assimp"]        = "%{wks.location}/Deps/assimp"
    deps["shaderc"]       = "%{wks.location}/Deps/shaderc"
    deps["glslang"]       = "%{wks.location}/Deps/glslang"
    deps["vulkan"]        = "%{wks.location}/Deps/Vulkan-Headers"
    deps["spirv_tools"]   = "%{wks.location}/Deps/SPIRV-Tools"
    deps["spirv_headers"] = "%{wks.location}/Deps/SPIRV-Headers"
    deps["meshoptimizer"] = "%{wks.location}/Deps/meshoptimizer"

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

    group "Deps"
        include "Deps/glad"
        include "Deps/glfw"
        include "Deps/assimp"
        -- include "Deps/shaderc"
        -- include "Deps/glslang"
        -- include "Deps/spirv_tools"
        -- include "Deps/spirv_cross"
        include "Deps/meshoptimizer"
    group ""
