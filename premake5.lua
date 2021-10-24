-- Copyright 2021 SMS
-- License(Apache-2.0)

include "thirdparty/premake/solution_items.lua"

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
thirdparty["assimp"] = "%{wks.location}/thirdparty/assimp"
thirdparty["vulkan"] = "%{wks.location}/thirdparty/Vulkan-Headers"

outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "Source"
include "Examples"

-- include "Thirdparty/assimp"