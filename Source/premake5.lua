-- Copyright 2021 SMS
-- License(Apache-2.0)

project "Source"
  kind "StaticLib"
  language "C++"
  cppdialect "C++latest"
  staticruntime "on"

  targetdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/lib")
  objdir("%{wks.location}/build/" .. outputdir .. "/%{prj.name}/obj")

  files {"**.cpp", "**.hpp", "**.h", "**.inl", "premake5.lua"}

  includedirs {
    "%{thirdparty.stb}",
    "%{thirdparty.math}/include",
    "%{thirdparty.glad}/include",
    "%{thirdparty.glfw}/include",
    "%{thirdparty.assimp}/include",
    "%{thirdparty.vulkan}/include",
    "%{thirdparty.meshoptimizer}/src"}

  links {
    "glad",
    "glfw",
    "meshoptimizer",
    "%{thirdparty.assimp}/build/lib/Debug/assimp-vc142-mtd",
    -- "%{thirdparty.vulkan}/lib/vulkan-1",
    -- "%{thirdparty.openal}/lib/openal32",
  }
