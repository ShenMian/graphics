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
thirdparty["glad"] = "%{wks.location}/Thirdparty/glad"
thirdparty["glfw"] = "%{wks.location}/Thirdparty/glfw"
thirdparty["assimp"] = "%{wks.location}/Thirdparty/assimp"
thirdparty["vulkan"] = "%{wks.location}/Thirdparty/Vulkan-Headers"
thirdparty["meshoptimizer"] = "%{wks.location}/Thirdparty/meshoptimizer"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Source"
include "Examples"

include "Thirdparty/glad"
include "Thirdparty/glfw"
include "Thirdparty/meshoptimizer"
-- include "Thirdparty/assimp"
