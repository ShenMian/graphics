-- Copyright 2021 SMS
-- License(Apache-2.0)

include "Deps/premake/solution_items.lua"

workspace "Graphics"
    architecture "x86_64"
    startproject "Triangle"
    configurations {"Debug", "Release"}
    flags "MultiProcessorCompile"

    output_dir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

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
    -- openmp "On"
    -- filter "toolset:not msc*"
    --     buildoptions "-fopenmp"

    -- 第三方库路径
    deps = {}
    deps["fmt"]               = "%{wks.location}/Deps/fmt"
    deps["stb"]               = "%{wks.location}/Deps/stb"
    deps["math"]              = "%{wks.location}/Deps/Math"
    deps["glad"]              = "%{wks.location}/Deps/glad"
    deps["glfw"]              = "%{wks.location}/Deps/glfw"
    deps["imgui"]             = "%{wks.location}/Deps/imgui"
    deps["assimp"]            = "%{wks.location}/Deps/assimp"
    deps["shaderc"]           = "%{wks.location}/Deps/shaderc"
    deps["glslang"]           = "%{wks.location}/Deps/glslang"
    deps["imguizmo"]          = "%{wks.location}/Deps/ImGuizmo"
    deps["spirv_tools"]       = "%{wks.location}/Deps/SPIRV-Tools"
    deps["vk_bootstrap"]      = "%{wks.location}/Deps/vk-bootstrap"
    deps["spirv_headers"]     = "%{wks.location}/Deps/SPIRV-Headers"
    deps["meshoptimizer"]     = "%{wks.location}/Deps/meshoptimizer"
    deps["imgui_node_editor"] = "%{wks.location}/Deps/imgui-node-editor"

    -- 第三方库头文件路径
    deps_inc = {}
    deps_inc["fmt"]               = "%{deps.fmt}/include"
    deps_inc["stb"]               = "%{deps.stb}"
    deps_inc["math"]              = "%{deps.math}/include"
    deps_inc["glad"]              = "%{deps.glad}/include"
    deps_inc["glfw"]              = "%{deps.glfw}/include"
    deps_inc["imgui"]             = "%{deps.imgui}"
    deps_inc["assimp"]            = "%{deps.assimp}/include"
    deps_inc["vulkan"]            = "%{wks.location}/Deps/Vulkan-Headers/include"
    deps_inc["imguizmo"]          = "%{wks.location}/Deps/ImGuizmo"
    deps_inc["vk_bootstrap"]      = "%{deps.vk_bootstrap}/src"
    deps_inc["meshoptimizer"]     = "%{deps.meshoptimizer}/src"
    deps_inc["imgui_node_editor"] = "%{deps.imgui_node_editor}"
    -- deps_inc["shaderc"] = "%{deps.shaderc}/libshaderc/include"

    deps_lib = {}
    deps_lib["vulkan"] = "%{wks.location}/Deps/VulkanSDK/Lib/vulkan-1"

    include "Source"

    group "Examples"
        include "Examples/Triangle"
        include "Examples/Cube"
        include "Examples/Viewer"
    group ""

    group "Deps"
        include "Deps/glad"
        include "Deps/glfw"
        include "Deps/imgui"
        include "Deps/assimp"
        include "Deps/imguizmo"
        include "Deps/vk_bootstrap"
        include "Deps/meshoptimizer"
        include "Deps/imgui_node_editor"
        -- include "Deps/shaderc"
        -- include "Deps/glslang"
        -- include "Deps/spirv_tools"
        -- include "Deps/spirv_cross"
    group ""
