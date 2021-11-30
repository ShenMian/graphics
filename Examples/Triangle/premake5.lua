-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Triangle"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++latest"
  staticruntime "on"

	files {"**.cpp", "**.hpp", "**.h", "**.inl", "CMakeLists.txt", "premake5.lua"}

  includedirs {
    "%{wks.location}/Source",
    "%{deps_inc.math}",
    "%{deps_inc.assimp}",
    "%{deps_inc.vulkan}"}

  links "Graphics"

  postbuildcommands "{COPYDIR} \"Shaders\" \"%{cfg.targetdir}\""
