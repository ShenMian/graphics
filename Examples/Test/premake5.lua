-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Test"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++latest"
  staticruntime "on"

  targetdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/bin")
  objdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/obj")

  files {"**.cpp", "**.hpp", "**.h", "**.inl", "premake5.lua"}

  includedirs {
    "%{wks.location}/Source",
    "%{thirdparty.math}/include",
    "%{thirdparty.assimp}/include",
    "%{thirdparty.vulkan}/include"}

  links "Source"
