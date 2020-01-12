workspace "RTR_Solution"

	-- System architecture (either 32 or 64 bit)
	architecture "x86"
	startproject "RTR_Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

-- Output directories
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory), cpp compiler include directories
IncludeDir = {}
IncludeDir["SPDLOG"] = "external/SPDLOG/include"
IncludeDir["GLFW"] = "external/GLFW/include"
IncludeDir["GLEW"] = "external/GLEW/include"
IncludeDir["ASSIMP"] = "external/ASSIMP/include"
IncludeDir["GLM"] = "external/GLM/glm"
IncludeDir["STB_IMAGE"] = "external/STB/include"

-- Include other premake files
include "external/GLFW"


project "RTR_Engine"
	location "RTR_Engine"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	-- Precompiled header
	pchheader "rtrpch.h"
	pchsource "RTR_Engine/src/rtrpch.cpp"

	-- Adds files to a project
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"external/STB/include/**.h",
		"external/STB/include/**.cpp",
	}

	-- Specifies the include file search paths for the compiler
	includedirs
	{
		"%{prj.name}/src",			-- Included so that headers can be included from the source
		"%{IncludeDir.SPDLOG}",	
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB_IMAGE}"
	}

	-- Sets additional dependencies (.lib files)
	links 
	{ 
		"opengl32.lib",	
		"glew32s.lib",
		"GLFW",
		"assimp-vc141-mtd"
	}

	 -- For 32 bit use these library paths
    libdirs { 
		"external/ASSIMP/lib",
		"external/GLEW/lib/win32",
	 }

	-- Build settings for windows
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS",
			"RTR_BUILD_DLL"
		}

		postbuildcommands
		{
			-- copies RTR_Engine.dll into RTR_Sandbox executable folder
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/RTR_Sandbox")
		}

	-- Build configurations
	filter "configurations:Debug"
		defines "RTR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "RTR_DISTRIBUTION"
		runtime "Release"
		optimize "on"


project "RTR_Sandbox"
	location "RTR_Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	-- Specifies the include file search paths for the compiler
	includedirs
	{
		"%{IncludeDir.SPDLOG}",
		"%{IncludeDir.ASSIMP}",
		"RTR_Engine/src"
	}

	links 
	{
		"RTR_Engine"
	}

	-- Build settings for windows
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS"
		}

		postbuildcommands
		{
			-- copies assimp-vc141-mtd.dll into RTR_Sandbox executable folder
			("{COPY} %{wks.location}external/ASSIMP/lib/assimp-vc141-mtd.dll %{cfg.targetdir}")
		}


	-- Build configurations
	filter "configurations:Debug"
		defines "RTR_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RTR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "RTR_DISTRIBUTION"
		runtime "Release"
		optimize "on"