workspace "RTR_Solution"

	-- System architecture (either 32 or 64 bit)
	architecture "x86"
	startproject "RTR_WinterWonderland"

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
IncludeDir["SPDLOG"] = "RTR_Engine/external/SPDLOG/include"
IncludeDir["GLFW"] = "RTR_Engine/external/GLFW/include"
IncludeDir["GLEW"] = "RTR_Engine/external/GLEW/include"
IncludeDir["ASSIMP"] = "RTR_Engine/external/ASSIMP/include"
IncludeDir["GLM"] = "RTR_Engine/external/GLM"
IncludeDir["STB_IMAGE"] = "RTR_Engine/external/STB/include"
IncludeDir["IMGUI"] = "RTR_Engine/external/imgui"

-- Include other premake files
group "Dependencies"
	include "RTR_Engine/external/GLFW"
	include "RTR_Engine/external/IMGUI"
group ""

project "RTR_Engine"
	location "RTR_Engine"
	kind "Staticlib"
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
		"%{prj.name}/external/STB/include/**.h",
		"%{prj.name}/external/STB/include/**.cpp",
		"%{prj.name}/external/GLM/glm/**.hpp",
		"%{prj.name}/external/GLM/glm/**.inl",
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
		"%{IncludeDir.STB_IMAGE}",
		"%{IncludeDir.IMGUI}",
	}

	-- Sets additional dependencies (.lib files)
	links 
	{ 
		"opengl32.lib",	
		"glew32s.lib",
		"GLFW",
		"assimp-vc141-mtd",
		"IMGUI",
	}

	 -- For 32 bit use these library paths
	libdirs { 
		"RTR_Engine/external/ASSIMP/lib",
		"RTR_Engine/external/GLEW/lib/win32",
	 }

	-- Build settings for windows
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"RTR_PLATFORM_WINDOWS",
			"RTR_BUILD_DLL"
		}

	-- Build configurations (runtime sets runtime library to /MDd for Debug and /MD to all other configs)
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


project "RTR_WinterWonderland"
	location "RTR_WinterWonderland"
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
		"%{prj.name}/src",			-- Included so that headers can be included from the source
		"RTR_Engine/src",
		"%{IncludeDir.SPDLOG}",	
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.ASSIMP}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.STB_IMAGE}",
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
			-- copies assimp-vc141-mtd.dll into RTR_WinterWonderland executable folder
			("{COPY} %{wks.location}RTR_Engine/external/ASSIMP/lib/assimp-vc141-mtd.dll %{cfg.targetdir}")
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